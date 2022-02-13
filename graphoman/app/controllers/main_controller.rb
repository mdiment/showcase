require "./parser_wrapper"

class MainController < ApplicationController
  before_action :parse_params, only: :result

  def input
    @tables = Table.where(user_id: current_user.id)
  end

  def result
    current_user_id = current_user.id
    record = Table.where(user_id: current_user_id).find_by(func: @func_string, lower_x: @lower_x, upper_x: @upper_x)
    if record.nil?
      func = Parser.new.make_f(@func_string)
      step = 1.0 * (@upper_x - @lower_x) / @number_of_points
      max = func.calculate(@lower_x)
      min = max
      data = (0..@number_of_points).map do |iteration|
        x_iter = @lower_x + iteration * step
        begin
          y = func.calculate(x_iter)
          max = y if y > max
          min = y if y < min
          [x_iter, y]
        rescue ArgumentError
          [x_iter, nil]
        end
      end

      @data_sum = []
      prev_i = -1
      for i in data.each_index.select { |i| data[i].last == nil or i == data.count - 1 }
        prev_i += 1
        @data_sum += data[prev_i, i - prev_i] if i != prev_i
        prev_i = i
      end

      if @data_sum
        record = Table.create(func: @func_string, result: @data_sum, lower_x: @lower_x, upper_x: @upper_x, user_id: current_user_id)
        @tables = Table.where(user_id: current_user_id)
      else
        @alert = "Множество значений функции #{@func_string} на отрезке [#{@lower_x}, #{@upper_x}] является пустым."
      end
    else 
      data = record[:result].scan(/\-?\d+\.?\d+/).map { |x| x.to_f }
      size = data.size
      max = data[1]
      min = max
      @data_sum = (0..size - 2).inject([]) do |acc, i|
        y = data[i + 1]
        max = y if y > max
        min = y if y < min
        i % 2 == 0 ? acc << [data[i], y] : acc
      end
      @tables = Table.where(user_id: current_user_id)
    end

    respond_to do |format|
      format.json do
        render json: { tables: @tables, dataset: @data_sum, 
                      func: @func_string, x_min: @lower_x, x_max: @upper_x,
                      y_max: max, y_min: min }
      end
    end
  end

  private
  def parse_params
    @func_string = params[:func_string]
    @lower_x = params[:lower_x].to_f
    @upper_x = params[:upper_x].to_f

    @number_of_points = 1000
  end
end

class CreateTables < ActiveRecord::Migration[6.1]
  def change
    create_table :tables do |t|
      t.string :func
      t.string :result
      t.integer :lower_x
      t.integer :upper_x

      t.timestamps
    end
  end
end

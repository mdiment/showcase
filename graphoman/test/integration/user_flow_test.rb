require "test_helper"

class UserFlowTest < ActionDispatch::IntegrationTest
  test "registration of new user, enter, calculate smth and logout" do
    get root_url
    assert_response :redirect
    follow_redirect!
    assert_response :success
    assert_select "h1", text: 'Авторизация'

    get new_user_path
    assert_select "h1", text: 'New User'

    post '/users', params: { user: { username: 'test', password: '123', password_confirmation: '123' } }
    assert_response :redirect
    follow_redirect!
    assert_response :success
    assert_select '#notice', text: 'User was successfully created.'

    get root_url
    assert_response :success
    assert_select "input[type='number']", { count: 1 }
    assert_select "input[type='submit']", { count: 1 }

    post '/main/result', params: { func_string: "sin(1/x)", lower_x: "-10", upper_x: "10" }
    assert_select "h1", text: 'График'
    assert_select "#chart-1"
    assert_select "tr", 1 + 1

    get '/session/logout'
    assert_response :redirect
    follow_redirect!
    assert_response :success
    assert_select "h1", text: 'Авторизация'
  end

end

Rails.application.routes.draw do
  resources :tables
  resources :users
  root 'main#input'
  match 'main/result', via: %i[get post], as: :main_result

  get 'session/login'
  post 'session/create'
  get 'session/logout'
  # For details on the DSL available within this file, see https://guides.rubyonrails.org/routing.html
end

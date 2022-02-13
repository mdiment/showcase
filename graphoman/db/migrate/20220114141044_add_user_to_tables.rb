class AddUserToTables < ActiveRecord::Migration[6.1]
  def change
    add_reference :tables, :user, null: false, foreign_key: true
  end
end

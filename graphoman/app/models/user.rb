class User < ApplicationRecord
  has_many :tables

  validates :username, presence: true, uniqueness: true

  has_secure_password

  def update_last_login_at
    self.last_login_at = DateTime.now
    save
  end
end

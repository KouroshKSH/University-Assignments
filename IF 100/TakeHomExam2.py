# Get database
db = input("Please enter the database: ")

# Divide the accounts by comma
account_list = db.split(',')
# account_list = account_list.split

# Since each account has:
# ... username, type, follower, following
# ... divide them into 4 sections
account_div = [None] * len(account_list)
for idx in range(len(account_list)):
  account_div[idx] = account_list[idx].split(':')

# Create a list of usernames
users = [] * len(account_div)
for idx in range(len(account_div)):
  users.append(account_div[idx][0])

# Create a list of interests
interests = [] * len(account_div)
for idx in range(len(account_div)):
  interests.append(account_div[idx][1])

# Get username
username = input("Please enter the username: ")

# Encryption process in 3 steps
enc_user = username[len(username) // 2 :]
enc_user += username[(len(username) // 2) - 1].lower()
enc_user += username[: len(username) // 2]

# Check the encrypted username
if enc_user in users:
  key = users.index(enc_user)
  # Check for bot
  if int(account_div[key][3]) / int(account_div[key][2]) >= 10:
    print("There is a high chance that", username, "may be a bot!")
  else:
    # Get interest type
    inter_type = input("Please enter the type you are interested in: ")

    # Distinguish interests via dashes
    specific_interest = interests[key].split("-")

    # Check for matching interests
    if inter_type in specific_interest:
      print("It is safe to accept the following request of ", username, ".", sep = "")
    else:
      print("Interests do not match with ", username, ".", sep = "")
else:
  print("Username does not exist!")
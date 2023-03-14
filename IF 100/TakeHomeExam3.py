# get food dataset and split by comma
foods_ds_input = input("Please enter the dataset you have: ")
food_db = foods_ds_input.split(',')

# create groups and fill them
possible_food = []
possible_food_lower = [] #will be used later
food_groups = []
food_cal = []

for food in food_db:
  possible_food.append(food.split(':')[0])
  possible_food_lower.append(food.split(':')[0].lower())
  food_groups.append(food.split(':')[1])
  # this list should later become a list of numbers, not strings
  food_cal.append(food.split(':')[2])

# make this list a list of numbers instead of strings
for i in range(len(food_cal)):
  # splitting by 'c' will work on both 'cals' and 'cal'
  food_cal[i] = float(food_cal[i].split('c')[0])


# for checking
# f = 1: eaten food(s) not in list
# f = 2: there's an alternative food
flag = 0


# get eaten food and convert to list
# should not convert the input to lowercase because ***
eaten_food = input("PLease enter the food(s) you ate during the day: ")
ate_list = eaten_food.split(',')

# check all eaten foods
# will become the list of invalid eaten foods to be printed
invalid_eaten_foods = []
for i in range(len(ate_list)):
  # ***: we want to print the invalid food like its initial form
  if ate_list[i].lower() not in possible_food_lower:
    invalid_eaten_foods.append(ate_list[i])
    flag = 1

if flag == 1:
  # 1st validity check failed
  temp1 = input("Please enter the amount of calories you can take in a day: ")
  temp2 = input("Please enter the food you want to eat: ")

  # show the eaten food(s) that doesn't exist in dataset
  for i in range(len(invalid_eaten_foods)):
    print(invalid_eaten_foods[i], "is not in the dataset.")
else:
  # 1st vaidity check was successful
  # get daily calorie limit as number
  cal_lim = float(input("Please enter the amount of calories you can take in a day: "))

  # get wanted food
  wanted_food = input("Please enter the food you want to eat: ")
  # should not convert to lowercase just like the previous time
  if wanted_food.lower() not in possible_food_lower:
    print(wanted_food, "does not exist in the dataset.")
  else:
    # calculate cal(s) of eaten food(s)
    tot_cal_eaten = 0
    for i in range(len(ate_list)):
      tot_cal_eaten += food_cal[possible_food_lower.index(ate_list[i].lower())]

    # check if it's possible to eat wanted food
    cal_taken = tot_cal_eaten + food_cal[possible_food_lower.index(wanted_food.lower())]
    if cal_taken < cal_lim:
      # there's no problem for eating the wanted food
      print("You can eat", wanted_food, "that you will get", format(cal_taken, '.0f'), "calories.")
    else:
      # check if there is another food from that group
      # store all of the foods within the same group in a list
      same_group_foods = []

      # since the 2 lists correspond 1-to-1
      group_name = food_groups[possible_food_lower.index(wanted_food.lower())]
      for i in range(len(food_groups)):
        if group_name == food_groups[i]:
          same_group_foods.append(possible_food[i])

      # check if a valid alternative exists
      # if so, print the very first one
      cal_taken = tot_cal_eaten
      for i in range(len(same_group_foods)):
        if cal_taken + food_cal[possible_food.index(same_group_foods[i])] < cal_lim:
          alt_food = same_group_foods[i]
          flag = 2
          break

      if flag == 2:
        print("You cannot eat ", wanted_food, ". However, we are suggesting you to eat ", alt_food, ".", sep="")
      else:
        print("There is no food you can eat from", group_name, "group.")
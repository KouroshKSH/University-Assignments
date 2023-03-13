"""
Get inputs:
  1. Amount of tomatoes purchased from supplier in kilogram (float);
  2. Boiling speed in kilogram per second (float);
  3. Can volume in liters (float);
Calculate variables:
  1. Number of produced cans (int);
  2. Time elapsed in HHMMSS (int, int, float);
  3. Leftover and purchase fee (float, float);
Process:
  can_number := (tomato_input * 0.20) // can_volume;
  total_time := tomato_input / boiling_speed;
  time_hour := total_time // 3600;
  time_minute := (total_time - (time_hour * 3600)) // 60;
  time_second := total_time - (time_hour * 3600) - (time_minute * 60);
  leftover := (tomato_input * 0.2) - (can_number * can_volume);
  Since price of leftovers = 0.02 Turkish Liras per gram:
    cost := leftover * 1000 * price_leftover;
Print:
  1. 'can_number' number(s) of can is produced.
  2. 'time_hour' hour(s) 'time_minute' minute(s) 'time_second' second(s) is taken.
  3. Leftover amount is 'leftover'kg. You can purchase the leftover by giving 'cost' TL.
"""

# Get inputs based on their type.
tomato_input = float(input("Please enter the amount of tomatoes (in kg) that you have: "))
boiling_speed = float(input("Please enter the boiling speed (rate at which a kg of tomato boiled in a second (kg/sec)): "))
can_volume = float(input("Please enter the volume of a can: "))

# Don't forget to consider the 20% pulp!
can_number = (tomato_input * 0.20) // can_volume
total_time = tomato_input / boiling_speed

# Priority of calculation: hour > minute > second
time_hour = total_time // 3600
time_minute = (total_time - (time_hour * 3600)) // 60
time_second = total_time - (time_hour * 3600) - (time_minute * 60)

# The leftover must be calculated based on the pulp.
leftover = (tomato_input * 0.2) - (can_number * can_volume)
ratio_leftover = 0.02
cost = leftover * 1000 * ratio_leftover

print(format(can_number, ".0f"), "number(s) of can is produced.")
print(format(time_hour, ".0f"), "hour(s)", format(time_minute, ".0f"), "minute(s)", format(time_second, ".2f"), "second(s) is taken.")
print("Leftover amount is ", format(leftover, ".2f"), "kg. You can purchase the leftover by giving ", format(cost, ".2f"), " TL.", sep='')
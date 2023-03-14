def choose_side():
  player1_side_decision = input("Player1, please choose your side (either X or O): ")

  # receive infinitely if needed
  while player1_side_decision not in "XO":
    player1_side_decision = input("Player1, please choose your side (either X or O): ")
  
  return player1_side_decision


def display_enviroment(field):
  row_edges = "-+-+-"

  # traverse the 2D field
  for i in range(len(field)):
    for j in range(len(field)):

      # reaching the edge of columns
      if j == len(field) - 1:
        print(field[i][j], end="")
      else:
        print(field[i][j], end="|")

    # next line  
    print()

    # reaching the edge of rows
    if i == len(field) - 1:
      print("", end="")
    else:
      print(row_edges)


def makechoice(player_num):
  print("Player", player_num, ", please make your choice: ", sep="", end="")
  player_pos_decision = input()
  
  # it's expected to have `row,col` as string input
  row = int(player_pos_decision[0])
  col = int(player_pos_decision[2])

  player_choice = [row, col]
  return player_choice


def implychoices(field, player_choice, player_mark):
  row, col = int(player_choice[0]), int(player_choice[1])

  # update or show error
  if field[row][col] == " ":
    field[row][col] = player_mark
  else:
    print(field[row][col], 
          " is already in the location ", 
          row, ",", col, ". Skipping this turn.", sep="")
    
  return field


def checkboard(field):
  # there is no empty slot by default
  result = False

  # check all slots
  for i in range(len(field)):
    for j in range(len(field)):
      if field[i][j] == " ":
        # found slot, break immidiately
        result = True
        break

  return result


def is_winner(field):
  # checking horizontal
  for i in range(len(field)):
    if field[i][0] == field[i][1] == field[i][2]:
      if field[i][0] == "X":
        return "X"
      elif field[i][0] == "O":
        return "O"

  # checking vertical
  for i in range(len(field)):
    if field[0][i] == field[1][i] == field[2][i]:
      if field[0][i] == "X":
        return "X"
      elif field[0][i] == "O":
        return "O"

  # checking diagonal (both diagonals)
  if field[0][0] == field[1][1] == field[2][2]:
    if field[0][0] == "X":
      return "X"
    elif field[0][0] == "O":
      return "O"

  if field[0][2] == field[1][1] == field[2][0]:
    if field[0][2] == "X":
      return "X"
    elif field[0][2] == "O":
      return "O"

  return " "
  
# Main starts here basically 
print("Welcome to the Tic-Tac-Toe Game!")
player1 = choose_side()
if player1 == "X":
  player2 = "O"
else:
  player2 = "X"

field = [[" ", " ", " "], [" ", " ", " "], [" ", " ", " "]]
display_enviroment(field)

while True:
  player1_choice = makechoice("1")
  field = implychoices(field, player1_choice, player1)
  display_enviroment(field)
  if is_winner(field) == player1 or checkboard(field) == False:
    break
  
  player2_choice = makechoice("2")
  field = implychoices(field, player2_choice, player2)
  display_enviroment(field)
  if is_winner(field) == player2 or checkboard(field) == False:
    break

if checkboard(field) == False:
  print("The game is finished! It is a tie.")

else:
  if player1 == is_winner(field):
    print("Player1 has won the game!")

  else:
    print("Player2 has won the game!")
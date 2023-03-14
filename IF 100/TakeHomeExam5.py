main_file = open("songs.txt"); lines = main_file.readlines(); main_file.close(); genres = [line.strip()[2:].split(" | ")[1] for line in lines]
while True:
    genre = input("Please enter the genre that you want to listen: ")
    if genre in genres:
        songs, cand, pop, bpm, winner, parts = {}, [], 0, 0, "", [line.strip()[2:].split(" | ") for line in lines]
        for part in parts: songs[part[0]] = [int(part[2]), int(part[3])] if genre == part[1] else [pop, bpm]
        for song in songs:
            if songs[song][0] == pop: cand.append(song)
            elif songs[song][0] > pop: cand.clear(); cand.append(song); pop = songs[song][0]
        for song in cand: winner, bpm = (song, songs[song][1]) if songs[song][1] > bpm else (winner, bpm)
        print("You can listen", winner, "that has", pop, "popularity and", bpm, "beats per minute.")
    elif genre == "quit": print("The program is closed."); break
    else: print("There is no such genre ", genre, ".", sep="")
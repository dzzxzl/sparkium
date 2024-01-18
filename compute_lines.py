# open file a.txt

num = 0

with open("a.txt") as f:
    # read the file line by line
    for line in f:
        # read the first word of the line
        word = line.split()[0]
        word_2 = line.split()[1]
        # if line ends with .obj, discard it
        if line.endswith(".obj\n"):
            continue
        # print(word)
        # cast it to int
        if word.isdigit():
            num += int(word)
        if word_2.isdigit():
            num += int(word_2)
    print(num)
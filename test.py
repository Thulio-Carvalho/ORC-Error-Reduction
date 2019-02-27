import time
import sys
import Util

with open("lexicon.txt") as f: lexicon = f.readlines()
## lexicon = [line.split() for line in lexicon]

with open("train.txt") as f: train = f.readlines()
train_words = [line.split() for line in train]

print 'Finished reading'

cnt = 0

paragraph = train[0]

intervals = []

for i in range(len(lexicon)):

    time_start = time.time()
    # string = ("%02f" % (i / float(len(lexicon))))
    # sys.stdout.write(string)
    # sys.stdout.flush()
    # sys.stdout.write("\b" * len(string))


    line = lexicon[i]
    found_match = False
    kmp = Util.KMP(line, paragraph)

    start = kmp.search()


    if (start != -1):
        intervals.append((start, start + len(line)))
        found_match = True

        cnt += 1
        sys.stdout.write("MATCHES: " + str(cnt))
        sys.stdout.flush()
        sys.stdout.write("\b" * len("MATCHES: " + str(cnt)))
            
    time_end = time.time()

    time_elapsed = (time_end - time_start)
    time_string = "time elapsed: %20f" % time_elapsed

    sys.stdout.write(time_string)
    sys.stdout.flush()
    sys.stdout.write("\b" * len(time_string))

match_indexes = [False] * len(paragraph)

for interval in intervals:
    for i in range(interval[0], interval[1] + 1):
        match_indexes[i] = True

for i in range(len(match_indexes)):
    if not match_indexes[i]:
        print i
            
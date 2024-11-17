./addecho -d 12000 -v 2 TESTS_GIVEN/door.wav test.wav
cmp -l test.wav TESTS_GIVEN/door_12000_2.wav
./addecho -d 12000 -v 4 TESTS_GIVEN/door.wav test.wav
cmp -l test.wav TESTS_GIVEN/door_12000_4.wav
./addecho -d 20000 -v 4 TESTS_GIVEN/door.wav test.wav
cmp -l test.wav TESTS_GIVEN/door_20000_4.wav
./addecho -d 35000 -v 2 TESTS_GIVEN/door.wav test.wav
cmp -l test.wav TESTS_GIVEN/door_35000_2.wav
./addecho -d 60000 -v 1 TESTS_GIVEN/door.wav test.wav
cmp -l test.wav TESTS_GIVEN/door_60000_1.wav
./addecho -d 3 -v 2 TESTS_GIVEN/short.wav test.wav
cmp -l test.wav TESTS_GIVEN/short_3_2.wav
rm test.wav
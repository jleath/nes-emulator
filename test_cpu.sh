make
./nes6502 < ./testing/nestest_commands > ./testing/nestest_actual.out
node ./testing/extract_codes.js
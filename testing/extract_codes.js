const fs = require('fs');

const extract_commands = () => {
  const data = fs.readFileSync('./testing/nestest.in', 'utf8');
  const lines = data.split('\n').map(line => {
    line = line.slice(6);
    line = line.slice(0, 10);
    return line.trim();
  });

  fs.writeFileSync('./testing/nestest_commands', lines.join('\n'));
};

const reformat_output = () => {
  const data = fs.readFileSync('./testing/nestest.in', 'utf8');
  let lines = data.split('\n');
  lines = lines.map(line => {
    line = line.slice(0, 6) + line.slice(16);
    return line
      .replace(/\s+/g, ' ')
      .replace(/PPU: ?\d+\, *\d+ /gi, '')
      .toLowerCase();
  });
  fs.writeFileSync('./testing/nestest_expected.out', lines.join('\n'));
};

const compare_files = () => {
  const expected = fs.readFileSync('./testing/nestest_expected.out', 'utf8');
  const actual = fs.readFileSync('./testing/nestest_actual.out', 'utf8');
  const expectedLines = expected.split('\n');
  const actualLines = actual.split('\n');
  for (let i = 0; i < expectedLines.length; i++) {
    const line = expectedLines[i].toLowerCase();
    const actual = actualLines[i].toLowerCase();
    if (line != actual) {
      console.log('Error!');
      console.log('expected: ' + line);
      console.log('got:      ' + actual);
      console.log('on line ' + (i + 1));
      break;
    }
  }
}

extract_commands();
reformat_output();
compare_files();

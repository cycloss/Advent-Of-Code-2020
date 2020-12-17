import 'dart:io';
import 'package:tuple/tuple.dart';

void main() async {
  var ops = await File("day8.txt").readAsString().then((str) => str.split("\n")).then((lines) => lines.map((line) {
            var matches = RegExp("(\\w+) (\\+|-)(\\d+)").firstMatch(line);
            return Tuple3<String, int, bool>(matches.group(1), matches.group(2) == "+" ? int.parse(matches.group(3)) : -int.parse(matches.group(3)),false);
          }).toList());

  int acc = 0;
  for (var i = 0, tup = ops[i]; !tup.item3; ops[i++] = tup.withItem3(true), tup = ops[i]) {
    switch (tup.item1) {
      case "acc":
        acc += tup.item2;
        break;
      case "jmp":
        i += tup.item2 - 1;
    }
  }
  print(acc);
}

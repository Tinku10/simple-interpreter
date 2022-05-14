PROGRAM Main;

PROCEDURE Alpha(a : INTEGER; b : INTEGER);
VAR x : INTEGER;
VAR y : REAL;

   PROCEDURE Beta(a : INTEGER; b : INTEGER);
   VAR x : INTEGER;
   BEGIN
      x := a * 10 + b * 2 + y;
      y := 3.1;
   END;

BEGIN
   x := (a + b ) * 2;
   y := 1.1 + 3.2;


   Beta(x, 1);      { procedure call }

END;

BEGIN { Main }

   Alpha(3 + 5, 7);  { procedure call }

END.  { Main }

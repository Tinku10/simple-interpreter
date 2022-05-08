PROGRAM Main;

PROCEDURE Alpha(a : INTEGER; b : INTEGER);
VAR x : INTEGER;
VAR y : INTEGER;

   PROCEDURE Beta(a : INTEGER; b : INTEGER);
   VAR x : INTEGER;
   BEGIN
      x := a * 10 + b * 2 + y;
   END;

BEGIN
   x := (a + b ) * 2;
   y := 1;


   Beta(5, 10);      { procedure call }
END;

BEGIN { Main }

   Alpha(3 + 5, 7);  { procedure call }

END.  { Main }

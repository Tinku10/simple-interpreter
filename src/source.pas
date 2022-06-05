PROGRAM Main;

PROCEDURE Alpha(a : INTEGER; b : INTEGER);
VAR x : INTEGER;
VAR y : REAL;
VAR z : STRING;

   PROCEDURE Beta(a : INTEGER; b : INTEGER);
   VAR x : INTEGER;
   VAR t : BOOLEAN;
   BEGIN
      x := a * 10 + b * 2 + y;
      y := 3.1;
      t := 1 > (2 - 2);
   END;

BEGIN
   x := (a + b ) * 2.5;
   y := 1.1 + 3.2;
   z := "hello" + " strings";

   IF (1 < 0) THEN
   BEGIN
     x := 0;
     Beta(x, 0);      { procedure call }
   END
   ELSE
   BEGIN
     x := 3;
   END;


END;

BEGIN { Main }

   Alpha(3 + 5, 0);  { procedure call }

END.  { Main }

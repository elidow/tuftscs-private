(* constraints.sml *)

val constraints = 
  [
   (* has solution *)
   CONAPP (TYCON "list", [TYVAR "int", TYVAR "a"]) ~ TYVAR "b" /\ 
   CONAPP (TYCON "list", [CONAPP (TYCON "list", [TYVAR "bool"])]) ~ 
   CONAPP (TYCON "list", [CONAPP (TYCON "list", [TYVAR "a"])]) /\ 
   TRIVIAL /\ CONAPP (TYCON "list", [TYVAR "b"]) ~
   CONAPP (TYCON "list", [CONAPP (TYCON "list", [TYVAR "int", TYVAR "bool"])]),
   (* no solution *)
   TYVAR "a" ~ TYVAR "bool" /\ 
   TYVAR "b" ~ CONAPP (TYCON "list", [TYVAR "bool"]) /\
   TYVAR "c" ~ CONAPP (TYCON "list", [TYVAR "bool"]) /\
   TRIVIAL /\
   TYVAR "a" ~ TYVAR "b",
   (* no solution *)
   CONAPP (TYCON "list", [TYVAR "int"]) ~ 
   CONAPP (TYCON "list", [TYVAR "int"]) /\ 
   TYCON "bool" ~ TYVAR "b" /\ TRIVIAL /\ 
   CONAPP (TYCON "list", [TYVAR "bool", TYVAR "b"]) ~ TYVAR "a" /\ 
   CONAPP (TYCON "list", [TYVAR "int", TYVAR "a"]) ~ TYVAR "a"]
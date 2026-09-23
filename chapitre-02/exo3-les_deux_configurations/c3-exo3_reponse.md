# Comparons les deux configurations 

## -- Debug :
Temps de construction : Days              : 0
                        Hours             : 0
                        Minutes           : 0
                        Seconds           : 1
                        Milliseconds      : 4
                        Ticks             : 10044185
                        TotalDays         : 1,16252141203704E-05
                        TotalHours        : 0,000279005138888889
                        TotalMinutes      : 0,0167403083333333
                        TotalSeconds      : 1,0044185
                        TotalMilliseconds : 1004,4185

Taille : 49170 octets

## -- Release :
Temps de construction : Days              : 0
                        Hours             : 0
                        Minutes           : 0
                        Seconds           : 1
                        Milliseconds      : 34
                        Ticks             : 10341732
                        TotalDays         : 1,19695972222222E-05
                        TotalHours        : 0,000287270333333333
                        TotalMinutes      : 0,01723622
                        TotalSeconds      : 1,0341732
                        TotalMilliseconds : 1034,1732

Taille : 49170 octets

## -- Les hash :
Get-FileHash "C:\Users\DELL\Desktop\Projet\projet\Build\Bin\Debug-Windows\projet\projet.exe"

Algorithm       Hash                                                                   Path        
---------       ----                                                                   ----        
SHA256          D674166B222B45E783FC1FEA7DBB6DF5C6A303E8FDF82EE140E76B4B6B032A59       C:\Users\...

Get-FileHash "C:\Users\DELL\Desktop\Projet\projet\Build\Bin\Release-Windows\projet\projet.exe"

Algorithm       Hash                                                                   Path        
---------       ----                                                                   ----        
SHA256          3B554AEDD12FE99F1C319699048A78EF5CA5A4F5B2EC3937C2EB11EEEC460196       C:\Users\...

## Conclusion : Nous pouvons remarquer, que les deux tailles sont identiques (49170 octets), malgré des hash différents (donc un contenu binaire différent à l'intérieur). C'est une coïncidence possible sur un projet aussi petit — avec très peu de code, l'écart typique entre Debug et Release (symboles de débogage en plus, optimisations en moins) peut ne représenter que quelques octets, qui s'annulent par hasard avec d'autres petites différences (alignement, métadonnées PE). Sur un projet plus conséquent, cet écart de taille se creuserait nettement, ce qui n'est pas le cas pour notre projet minimal.

De même, l'écart de temps de construction (1034 ms vs 1004 ms) est minime, puisqu'il s'agit  d'un projet à un seul petit fichier source : les vraies optimisations coûteuses du compilateur ne se font sentir sur le temps de build qu'à partir d'une base de code plus large. Sur un projet plus gros, on s'attendrait à un temps de Release nettement supérieur à celui de Debug, pas inférieur ou égal.

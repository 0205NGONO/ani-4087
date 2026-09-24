## Clé de signature :

**Commande employée pour fabriquer la clé de signature :**
```
keytool -genkeypair -v -keystore projet2-release-key.jks -alias projet2 -keyalg RSA -keysize 2048 -validity 10000
```
## Sortie :

```
keytool -genkeypair -v -keystore projet2-release-key.jks -alias projet2 -keyalg RSA -keysize 2048 -validity 10000
Entrez le mot de passe du fichier de clés :  

Ressaisissez le nouveau mot de passe : 

Enter the distinguished name. Provide a single dot (.) to leave a sub-component empty or press ENTER to use the default value in braces.
Quels sont vos nom et prénom ?
  [Unknown]:  NGONO AMBASSA Agnès Marie France
Quel est le nom de votre unité organisationnelle ?
  [Unknown]:  RIHEN Academy
Quel est le nom de votre entreprise ?
  [Unknown]:  RIHEN Academy
Quel est le nom de votre ville de résidence ?
  [Unknown]:  Yaoundé
Quel est le nom de votre état ou province ?
  [Unknown]:  Cameroun
Quel est le code pays à deux lettres pour cette unité ?
  [Unknown]:  +237
Est-ce CN=NGONO AMBASSA Agn?s Marie France, OU=RIHEN Academy, O=RIHEN Academy, L=Yaound?, ST=Cameroun, C="+237" ?
  [non]:  
Enter the distinguished name. Provide a single dot (.) to leave a sub-component empty or press ENTER to use the default value in braces.
Quels sont vos nom et prénom ?
  [NGONO AMBASSA Agn?s Marie France]:  NGONO AMBASSA AGNES MARIE FRANCE
Quel est le nom de votre unité organisationnelle ?
  [RIHEN Academy]:  UTUDIANTE
Quel est le nom de votre entreprise ?
  [RIHEN Academy]:  
Quel est le nom de votre ville de résidence ?
  [Yaound?]:  YAOUNDE
Quel est le nom de votre état ou province ?
  [Cameroun]:  
Quel est le code pays à deux lettres pour cette unité ?
  [+237]:  
Est-ce CN=NGONO AMBASSA AGNES MARIE FRANCE, OU=UTUDIANTE, O=RIHEN Academy, L=YAOUNDE, ST=Cameroun, C="+237" ?
  [non]:  OUI

Generating 2048-bit RSA key pair and self-signed certificate (SHA384withRSA) with a validity of 10?000 days
        for: CN=NGONO AMBASSA AGNES MARIE FRANCE, OU=UTUDIANTE, O=RIHEN Academy, L=YAOUNDE, ST=Cameroun, C="+237"
[Stockage de projet2-release-key.jks]
```

## Vérification : 
```
ls *.jks


    Répertoire : C:\Users\DELL\Desktop\Projet\projet2


Mode                 LastWriteTime         Length Name                                                                                                                                                                      
----                 -------------         ------ ----                                                                                                                                                                      
-a----        24/09/2026     10:37           2810 projet2-release-key.jks
```

**Où le mot de passe est rangé :** dans un fichier texte sur mon Bureau (`cle_signature_projet2.txt`), sur mon bureau, en dehors du projet.
"C:\Users\DELL\Desktop\cle_signature_projet2.txt"

# PP8: Schätzung der Leistungsaufnahme der CPU anhand von Task-Typen
##Zur Reduzierung des Ressourceneinsatzes benötigen einige statische Scheduler Informationen über die Leistungsaufnahme der CPU, bei Ausführung einer Task, die mittels Benchmarking zu erheben sind. 
###Um den Aufwand zu reduzieren wurde ein Vorgehen zur Schätzung der Leistungsaufnahme der CPU, anhand von Task-Typen evaluiert und in drei Artefakten implementiert: 
####Der Tasktypeanalyzer erstellt die Suchkriterien von Task-Typen. 
####Der Apptaskanalyzer erstellt die Suchkriterien aus Tasks einer  Beispielanwendung. 
####Der Tasktypeestimator besteht aus den Modulen Taskmapper und Estimator. 
#####Der Taskmapper verwendet zwei Abbildungsverfahren, um Tasks einer Anwendung auf Task-Typen zu kategorisieren. 
#####Der Estimator schätzt die Leistungsaufnahme der einzelnen Anwendungstasks auf Grundlage der Task-Typen. 


Auf dem System muss make, cmake und git installiert sein.

Um die Anwendungen herunterzuladen, ist das Shellskript mit dem Befehl sh init.sh auszuführen!

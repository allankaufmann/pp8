# PP8: Schätzung der Leistungsaufnahme der CPU anhand von Task-Typen
Zur Reduzierung des Ressourceneinsatzes benötigen einige statische Scheduler Informationen über die Leistungsaufnahme der CPU, bei Ausführung einer Task, die mittels Benchmarking zu erheben sind. 
Um den Aufwand zu reduzieren wurde ein Vorgehen zur Schätzung der Leistungsaufnahme der CPU, anhand von Task-Typen evaluiert und in drei Artefakten implementiert: 
- Der Tasktypeanalyzer erstellt die Suchkriterien von Task-Typen. 
- Der Apptaskanalyzer erstellt die Suchkriterien aus Tasks einer  Beispielanwendung. 
- Der Tasktypeestimator besteht aus den Modulen Taskmapper und Estimator. 
Der Taskmapper verwendet zwei Abbildungsverfahren, um Tasks einer Anwendung auf Task-Typen zu kategorisieren. 
Der Estimator schätzt die Leistungsaufnahme der einzelnen Anwendungstasks auf Grundlage der Task-Typen. 

Um das Testsystem für den Betrieb der Anwendungen vorzubereiten, wird das Shellskript init.sh aufgerufen. 
Ggfls. muss vorher noch das Ausführungsrecht durch 'chmod +x init.sh' erteilt werden, anschließend wird es mit dem Befehl './init.sh' aufgerufen.

Mit dem Aufruf des init-Skriptes wird:
- make installiert
- der Benchmark epEBench ausgecheckt, gebaut und vorbereitet
- die Anwendung cpupower installiert
- die Anwendung edgedetection vorbereitet und gebaut
- die Anwendung cmake für den Build des Tasktypeestimator installiert
- der Build der drei Artefakte ausgeführt

1_tasktype_analyzer:
- Skript run_all_tasks.sh ausführen um Sequenzen für Task-Typen zu erstellen

2_apptask_analyzer:
- Skript run_all_tasks.sh ausführen um Sequenzen für Edgedetection zu erstellen

3_tasktype_estimator
- ./main - um die Anwendung zu starten
- run_checkpreconditions.sh - um Vorbedingungen vor Schätzung der Leistungsaufnahme zu überprüfen und zu protokollieren
- run_estimation.sh - um das Experiment zu starten

WICHTIG: Für die Durchführung des Experiments werden Root-Berechtigungen benötigt!

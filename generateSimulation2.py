import sys, os, random, copy

def main(numberOfTraces):
    
    HwHm = "/u/s/p/spati/CS752/traces/sets/HM_HW"
    HwSm = "/u/s/p/spati/CS752/traces/sets/SM_HW"
    SwHm = "/u/s/p/spati/CS752/traces/sets/HM_SW"

    traceArray = {'HwHm':[],'HwSm':[],'SwHm':[]}

    for filename in os.listdir(HwHm):
        if filename.endswith(".gz"): 
            filePath = os.path.join(HwHm, filename)
            traceArray['HwHm'].append(filePath)
        else:
            continue
    
    for filename in os.listdir(HwSm):
        if filename.endswith(".gz"): 
            filePath = os.path.join(HwSm, filename)
            traceArray['HwSm'].append(filePath)
        else:
            continue
    
    for filename in os.listdir(SwHm):
        if filename.endswith(".gz"): 
            filePath = os.path.join(SwHm, filename)
            traceArray['SwHm'].append(filePath)
        else:
            continue
    
    if(len(traceArray)==0):
        print("No traces were found. Check directoryPath.")
	return 
    
    print(str(len(traceArray['HwHm'])) + " HwHm traces found")
    print(str(len(traceArray['HwSm'])) + " HwSm traces found")
    print(str(len(traceArray['SwHm'])) + " SwHm traces found")
    
    generateSimulations(traceArray,numberOfTraces)

def generateSimulations(traceArray,numberOfTraces):
    
    #generate simulation directory
    pathOfPyScript = os.path.dirname(os.path.abspath(__file__))
    pathToSimulations = ""
    #generate the directory only if we can make at least one tracelist
    if(len(traceArray['HwHm'])>=numberOfTraces):
        for x in range(0,10000):
            pathToSimulations = pathOfPyScript + "/" + "simulation" + str(x)
            if not os.path.exists(pathToSimulations):
	        os.makedirs(pathToSimulations)
	        break
    else:
        print("numberOfTraces requested per tracelist is larger than the total traces found")
        return

    numberOfMasters = len(traceArray['HwHm'])/(numberOfTraces/2)
    masterTraces = copy.deepcopy(traceArray['HwHm'])
    traceArraySave = copy.deepcopy(traceArray)
    #create tracelist files
    for simNumber in range(0,numberOfMasters):
        hwhmPath = pathToSimulations + "/HwHm-HwHm" + str(simNumber)
        hwsmPath = pathToSimulations + "/HwHm-HwSm" + str(simNumber)
        swhmPath = pathToSimulations + "/HwHm-SwHm" + str(simNumber)
        traceArray = copy.deepcopy(traceArraySave)
        if(len(traceArray['HwHm']) < numberOfTraces):
            print ("Only able to create " + str(simNumber) + " tracelists")# from " + str(numberOfSims) + " requested")
            break
        
        if(len(traceArray['HwSm']) < numberOfTraces/2):
            print ("Only able to create " + str(simNumber) + " tracelists")# from " + str(numberOfSims) + " requested")
            break
        
        if(len(traceArray['SwHm']) < numberOfTraces/2):
            print ("Only able to create " + str(simNumber) + " tracelists")# from " + str(numberOfSims) + " requested")
            break
        hwhmFile = open(hwhmPath, "w")
        hwsmFile = open(hwsmPath, "w")
        swhmFile = open(swhmPath, "w")
        #write to master hwhse
        for traceNumber in range(0,numberOfTraces/2):
            randomTrace = masterTraces[random.randint(0,len(masterTraces)-1)]
            hwhmFile.write(randomTrace + "\n")
            hwsmFile.write(randomTrace + "\n")
            swhmFile.write(randomTrace + "\n")
            traceArray['HwHm'].remove(randomTrace) 
            masterTraces.remove(randomTrace) 
        #write to trace file
        for traceNumber in range(0,numberOfTraces/2):
            randomTraceHHH = traceArray['HwHm'][random.randint(0,len(traceArray['HwHm'])-1)]
            randomTraceHHS = traceArray['HwSm'][random.randint(0,len(traceArray['HwSm'])-1)]
            randomTraceHSH = traceArray['SwHm'][random.randint(0,len(traceArray['SwHm'])-1)]
            hwhmFile.write(randomTraceHHH + "\n")
            hwsmFile.write(randomTraceHHS + "\n")
            swhmFile.write(randomTraceHSH + "\n")
            traceArray['HwHm'].remove(randomTraceHHH)  
            traceArray['HwSm'].remove(randomTraceHHS) 
            traceArray['SwHm'].remove(randomTraceHSH) 
        hwhmFile.close()
        hwsmFile.close()
        swhmFile.close()
        if(simNumber == numberOfMasters-1):
            print("created " + str(simNumber+1) + " tracelists")
        
    print("Tracelists located in " + pathToSimulations)
    
#Command Line Call to script
if __name__ == "__main__":
    if(len(sys.argv) == 2):
    	numberOfTraces = int(sys.argv[1])
    	main(numberOfTraces);
    else:	
	print("python generateSimulation2.py numberOfTracesPerSimulation") 

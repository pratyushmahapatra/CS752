#resultParser - takes a target directory that contains .res files for parsing
#               and parses these files into individual CSVs. The CSVs will be
#               located in the same directory as the resultParser.py resides.
#
#To use: 
#   1.place the .res files you wish to parser into a folder 
#       (Ex: /project/tracesResFiles/*.res)
#   2.have the resultParser.py exist in the parent directory above 
#       (Ex: /project/resultParser.py)
#   3.run the script using the following: python resultParser.py targetDirectory
#       (Ex: python resultParser.py traceResFiles)

import sys, os
import collections
def main(targetDirectory):
    #print("Main:",targetDirectory);
    pathOfPyScript = os.path.dirname(os.path.abspath(__file__))
    #print("Main:",pathOfPyScript); 
    directoryPath = pathOfPyScript + "/"+ targetDirectory
    #print("Main:",directoryPath); 
    
    for filename in os.listdir(directoryPath):
        if filename.endswith(".csv"): 
            filePath = os.path.join(directoryPath, filename)
            print(filePath)
            parseResults(filePath,filename)
        else:
            continue

def parseResults(filePath,filename):
    #Open the result data file for data retrieval
    file = open(filePath, "r")
    #result dictonary for writing to result csv
    resultDictionary={}
    tableResultDictionary={}
    ###Start of looping through data###
    for line in file:
        #try/except for catching bad lines when column spliting
        try:            
            #create the array of elements
            columns = line.split("|")
            #remove white space with replace
            process = columns[0].replace(" ","")
            branchNum = columns[1].replace(" ","") 
            BI = columns[3].replace(" ","") 
            BIndex = columns[4].replace(" ","") 
            Gtable = columns[5].replace(" ","") 
            Gbank = columns[6].replace(" ","") 
            Gindex = columns[7].replace(" ","") 
        
            lineIndex = ""
            if int(BI) == 1:
                #BI 1 and bank is not needed so set to 0 and bindex
                lineIndex = "10" + BIndex

                #add/update table accessed in tableResultDictionary
                if 'BI' in tableResultDictionary:
                    tableResultDictionary['BI']+=1
                else:
                    tableResultDictionary['BI']=1

            elif int(Gtable) == 1:
                #Gtable is 1 so 0 and gbank and gindex
                lineIndex = "0" + Gbank  + Gindex
                #add/update table accessed in tableResultDictionary
                if Gbank in tableResultDictionary:
                    tableResultDictionary[Gbank]+=1
                else:
                    tableResultDictionary[Gbank]=1
                 
            else:
                print("ERROR: neither BI or Gtable were 1 at line ->" + line)
                continue

        except:
            print("ERROR: caught in exception at line ->" + line)
            continue

        #print(process,branchNum,"BI",BI,BIndex,"Gtable",Gtable,Gbank,Gindex)

        #add/update line index in resultDictionary
        if lineIndex in resultDictionary:
            if process in resultDictionary[lineIndex]:
                resultDictionary[lineIndex][process]+=1
            else:
                resultDictionary[lineIndex][process]=1
        else:
            resultDictionary[lineIndex]={}
            resultDictionary[lineIndex][process]=1     

    ###End of looping through data###
    file.close()
    
    #print(resultDictionary)    
    #write the result dictionary to a csv 
    csvName = filename[0:len(filename)-4] + "-Result.csv"
    csvFile = open(csvName,"w")

    csvFile.write('TABLE:NUMBER OF ACCESS')
    for table in sorted(tableResultDictionary):
        csvFile.write(','+table+':'+str(tableResultDictionary[table]))
    csvFile.write('\n')

    processTable = ['P0','P1','P2','P3','P4','P5']
    for lineIndex in sorted(resultDictionary):
        csvFile.write(lineIndex)
        for process in processTable:
            if process in sorted(resultDictionary[lineIndex]):
                #print(lineIndex,process,resultDictionary[lineIndex][process])
                csvFile.write(','+str(resultDictionary[lineIndex][process]))
            else:
                csvFile.write(',0')
        csvFile.write('\n')
    csvFile.close()
    
#Command Line Call to script
if __name__ == "__main__":
    #Retrieve Directory args
    targetDirectory = sys.argv[1]
    #Call Main
    main(targetDirectory);

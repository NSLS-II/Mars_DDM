#!../../bin/linux-arm/zDDM

## You may have to change zDDM to something else
## everywhere it appears in this file

< envPaths

## Environment specific macros
#< unique.hex
#< unique.xpd
< unique.test

epicsEnvSet("EPICS_CA_AUTO_ADDR_LIST", "NO")
epicsEnvSet("EPICS_CA_ADDR_LIST", "172.16.0.211")

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/zDDM.dbd"
zDDM_registerRecordDeviceDriver pdbbase
devI2CConfig(0,74,8)
devI2CConfig(1,8,8)
devI2CConfig(2,9,8)
##devSPIConfig(0,1,8)
devzDDMConfig(1,1,$(NELM))

## Load record instances
#dbLoadTemplate "db/user.substitutions"
dbLoadRecords("db/det1.db","P=$(P),R=$(R),NELM=$(NELM)")
dbLoadRecords "db/i2cDacs.db","P=$(P),R=$(R)")
dbLoadRecords "db/i2cAdcs.db","P=$(P),R=$(R)")
dbLoadRecords "db/Tmp100.db","P=$(P),R=$(R)")
dbLoadRecords "db/ZTMPAdcs.db","P=$(P),R=$(R)")
##dbLoadRecords "db/SpiDacs.db","user=det1"
dbLoadRecords "db/timer.db","P=$(P),R=$(R)")


## Set this to see messages from mySub
#var mySubDebug 1

## Run this to trace the stages of iocInit
#traceIocInit
var zDDMRecordDebug 0
var devzDDMdebug 0
var devI2CDebug 0

cd "${TOP}/iocBoot/${IOC}"
epicsEnvSet EPICS_CA_MAX_ARRAY_BYTES 7000000

#< save_restore.cmd


iocInit

#dbpr det1 4 >det1_pvs.txt
## Start any sequence programs
#seq sncExample, "user=peter"
dbpf $(P)$(R).IPADDR 10.66.211.64

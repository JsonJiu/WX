#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-128K_Hall_Onboard.mk)" "nbproject/Makefile-local-128K_Hall_Onboard.mk"
include nbproject/Makefile-local-128K_Hall_Onboard.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=128K_Hall_Onboard
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/AutoTest.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/AutoTest.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../Codes/app/app_pcusart_protocol.c ../../Codes/bsp/app_autocheck.c ../../Codes/app/app_ina219.c ../../Codes/app/app_irdausart_protocol.c ../../Codes/bsp/app_MeterUsart_protocol.c ../../Codes/bsp/CBB_MCU_308_ForGprs.X/bsp_mc308.c ../../Codes/bsp/CBB_MCU_308_ForGprs.X/lib_mc308.c ../../Codes/CBB_CPF/task_manage.c ../../Codes/CBB_CPF/timer_manage.c ../../Codes/CBB_CPF/user_manage.c ../../Codes/drv/m2m_uart/drv_uart.c ../../Codes/drv/ADC/drv_ad.c ../../Codes/general/temp.c ../../Codes/bsp/comm.c ../../Codes/CBB_CPF/main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1223384328/app_pcusart_protocol.o ${OBJECTDIR}/_ext/1223383274/app_autocheck.o ${OBJECTDIR}/_ext/1223384328/app_ina219.o ${OBJECTDIR}/_ext/1223384328/app_irdausart_protocol.o ${OBJECTDIR}/_ext/1223383274/app_MeterUsart_protocol.o ${OBJECTDIR}/_ext/1623924188/bsp_mc308.o ${OBJECTDIR}/_ext/1623924188/lib_mc308.o ${OBJECTDIR}/_ext/2117827500/task_manage.o ${OBJECTDIR}/_ext/2117827500/timer_manage.o ${OBJECTDIR}/_ext/2117827500/user_manage.o ${OBJECTDIR}/_ext/1567105643/drv_uart.o ${OBJECTDIR}/_ext/1474187024/drv_ad.o ${OBJECTDIR}/_ext/810413631/temp.o ${OBJECTDIR}/_ext/1223383274/comm.o ${OBJECTDIR}/_ext/2117827500/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1223384328/app_pcusart_protocol.o.d ${OBJECTDIR}/_ext/1223383274/app_autocheck.o.d ${OBJECTDIR}/_ext/1223384328/app_ina219.o.d ${OBJECTDIR}/_ext/1223384328/app_irdausart_protocol.o.d ${OBJECTDIR}/_ext/1223383274/app_MeterUsart_protocol.o.d ${OBJECTDIR}/_ext/1623924188/bsp_mc308.o.d ${OBJECTDIR}/_ext/1623924188/lib_mc308.o.d ${OBJECTDIR}/_ext/2117827500/task_manage.o.d ${OBJECTDIR}/_ext/2117827500/timer_manage.o.d ${OBJECTDIR}/_ext/2117827500/user_manage.o.d ${OBJECTDIR}/_ext/1567105643/drv_uart.o.d ${OBJECTDIR}/_ext/1474187024/drv_ad.o.d ${OBJECTDIR}/_ext/810413631/temp.o.d ${OBJECTDIR}/_ext/1223383274/comm.o.d ${OBJECTDIR}/_ext/2117827500/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1223384328/app_pcusart_protocol.o ${OBJECTDIR}/_ext/1223383274/app_autocheck.o ${OBJECTDIR}/_ext/1223384328/app_ina219.o ${OBJECTDIR}/_ext/1223384328/app_irdausart_protocol.o ${OBJECTDIR}/_ext/1223383274/app_MeterUsart_protocol.o ${OBJECTDIR}/_ext/1623924188/bsp_mc308.o ${OBJECTDIR}/_ext/1623924188/lib_mc308.o ${OBJECTDIR}/_ext/2117827500/task_manage.o ${OBJECTDIR}/_ext/2117827500/timer_manage.o ${OBJECTDIR}/_ext/2117827500/user_manage.o ${OBJECTDIR}/_ext/1567105643/drv_uart.o ${OBJECTDIR}/_ext/1474187024/drv_ad.o ${OBJECTDIR}/_ext/810413631/temp.o ${OBJECTDIR}/_ext/1223383274/comm.o ${OBJECTDIR}/_ext/2117827500/main.o

# Source Files
SOURCEFILES=../../Codes/app/app_pcusart_protocol.c ../../Codes/bsp/app_autocheck.c ../../Codes/app/app_ina219.c ../../Codes/app/app_irdausart_protocol.c ../../Codes/bsp/app_MeterUsart_protocol.c ../../Codes/bsp/CBB_MCU_308_ForGprs.X/bsp_mc308.c ../../Codes/bsp/CBB_MCU_308_ForGprs.X/lib_mc308.c ../../Codes/CBB_CPF/task_manage.c ../../Codes/CBB_CPF/timer_manage.c ../../Codes/CBB_CPF/user_manage.c ../../Codes/drv/m2m_uart/drv_uart.c ../../Codes/drv/ADC/drv_ad.c ../../Codes/general/temp.c ../../Codes/bsp/comm.c ../../Codes/CBB_CPF/main.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-128K_Hall_Onboard.mk dist/${CND_CONF}/${IMAGE_TYPE}/AutoTest.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ128GA308
MP_LINKER_FILE_OPTION=,-Tp24FJ128GA308.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1223384328/app_pcusart_protocol.o: ../../Codes/app/app_pcusart_protocol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1223384328 
	@${RM} ${OBJECTDIR}/_ext/1223384328/app_pcusart_protocol.o.d 
	@${RM} ${OBJECTDIR}/_ext/1223384328/app_pcusart_protocol.o.ok ${OBJECTDIR}/_ext/1223384328/app_pcusart_protocol.o.err 
	@${RM} ${OBJECTDIR}/_ext/1223384328/app_pcusart_protocol.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1223384328/app_pcusart_protocol.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1223384328/app_pcusart_protocol.o.d" -o ${OBJECTDIR}/_ext/1223384328/app_pcusart_protocol.o ../../Codes/app/app_pcusart_protocol.c    
	
${OBJECTDIR}/_ext/1223383274/app_autocheck.o: ../../Codes/bsp/app_autocheck.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1223383274 
	@${RM} ${OBJECTDIR}/_ext/1223383274/app_autocheck.o.d 
	@${RM} ${OBJECTDIR}/_ext/1223383274/app_autocheck.o.ok ${OBJECTDIR}/_ext/1223383274/app_autocheck.o.err 
	@${RM} ${OBJECTDIR}/_ext/1223383274/app_autocheck.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1223383274/app_autocheck.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1223383274/app_autocheck.o.d" -o ${OBJECTDIR}/_ext/1223383274/app_autocheck.o ../../Codes/bsp/app_autocheck.c    
	
${OBJECTDIR}/_ext/1223384328/app_ina219.o: ../../Codes/app/app_ina219.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1223384328 
	@${RM} ${OBJECTDIR}/_ext/1223384328/app_ina219.o.d 
	@${RM} ${OBJECTDIR}/_ext/1223384328/app_ina219.o.ok ${OBJECTDIR}/_ext/1223384328/app_ina219.o.err 
	@${RM} ${OBJECTDIR}/_ext/1223384328/app_ina219.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1223384328/app_ina219.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1223384328/app_ina219.o.d" -o ${OBJECTDIR}/_ext/1223384328/app_ina219.o ../../Codes/app/app_ina219.c    
	
${OBJECTDIR}/_ext/1223384328/app_irdausart_protocol.o: ../../Codes/app/app_irdausart_protocol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1223384328 
	@${RM} ${OBJECTDIR}/_ext/1223384328/app_irdausart_protocol.o.d 
	@${RM} ${OBJECTDIR}/_ext/1223384328/app_irdausart_protocol.o.ok ${OBJECTDIR}/_ext/1223384328/app_irdausart_protocol.o.err 
	@${RM} ${OBJECTDIR}/_ext/1223384328/app_irdausart_protocol.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1223384328/app_irdausart_protocol.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1223384328/app_irdausart_protocol.o.d" -o ${OBJECTDIR}/_ext/1223384328/app_irdausart_protocol.o ../../Codes/app/app_irdausart_protocol.c    
	
${OBJECTDIR}/_ext/1223383274/app_MeterUsart_protocol.o: ../../Codes/bsp/app_MeterUsart_protocol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1223383274 
	@${RM} ${OBJECTDIR}/_ext/1223383274/app_MeterUsart_protocol.o.d 
	@${RM} ${OBJECTDIR}/_ext/1223383274/app_MeterUsart_protocol.o.ok ${OBJECTDIR}/_ext/1223383274/app_MeterUsart_protocol.o.err 
	@${RM} ${OBJECTDIR}/_ext/1223383274/app_MeterUsart_protocol.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1223383274/app_MeterUsart_protocol.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1223383274/app_MeterUsart_protocol.o.d" -o ${OBJECTDIR}/_ext/1223383274/app_MeterUsart_protocol.o ../../Codes/bsp/app_MeterUsart_protocol.c    
	
${OBJECTDIR}/_ext/1623924188/bsp_mc308.o: ../../Codes/bsp/CBB_MCU_308_ForGprs.X/bsp_mc308.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1623924188 
	@${RM} ${OBJECTDIR}/_ext/1623924188/bsp_mc308.o.d 
	@${RM} ${OBJECTDIR}/_ext/1623924188/bsp_mc308.o.ok ${OBJECTDIR}/_ext/1623924188/bsp_mc308.o.err 
	@${RM} ${OBJECTDIR}/_ext/1623924188/bsp_mc308.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1623924188/bsp_mc308.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1623924188/bsp_mc308.o.d" -o ${OBJECTDIR}/_ext/1623924188/bsp_mc308.o ../../Codes/bsp/CBB_MCU_308_ForGprs.X/bsp_mc308.c    
	
${OBJECTDIR}/_ext/1623924188/lib_mc308.o: ../../Codes/bsp/CBB_MCU_308_ForGprs.X/lib_mc308.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1623924188 
	@${RM} ${OBJECTDIR}/_ext/1623924188/lib_mc308.o.d 
	@${RM} ${OBJECTDIR}/_ext/1623924188/lib_mc308.o.ok ${OBJECTDIR}/_ext/1623924188/lib_mc308.o.err 
	@${RM} ${OBJECTDIR}/_ext/1623924188/lib_mc308.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1623924188/lib_mc308.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1623924188/lib_mc308.o.d" -o ${OBJECTDIR}/_ext/1623924188/lib_mc308.o ../../Codes/bsp/CBB_MCU_308_ForGprs.X/lib_mc308.c    
	
${OBJECTDIR}/_ext/2117827500/task_manage.o: ../../Codes/CBB_CPF/task_manage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2117827500 
	@${RM} ${OBJECTDIR}/_ext/2117827500/task_manage.o.d 
	@${RM} ${OBJECTDIR}/_ext/2117827500/task_manage.o.ok ${OBJECTDIR}/_ext/2117827500/task_manage.o.err 
	@${RM} ${OBJECTDIR}/_ext/2117827500/task_manage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2117827500/task_manage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/2117827500/task_manage.o.d" -o ${OBJECTDIR}/_ext/2117827500/task_manage.o ../../Codes/CBB_CPF/task_manage.c    
	
${OBJECTDIR}/_ext/2117827500/timer_manage.o: ../../Codes/CBB_CPF/timer_manage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2117827500 
	@${RM} ${OBJECTDIR}/_ext/2117827500/timer_manage.o.d 
	@${RM} ${OBJECTDIR}/_ext/2117827500/timer_manage.o.ok ${OBJECTDIR}/_ext/2117827500/timer_manage.o.err 
	@${RM} ${OBJECTDIR}/_ext/2117827500/timer_manage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2117827500/timer_manage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/2117827500/timer_manage.o.d" -o ${OBJECTDIR}/_ext/2117827500/timer_manage.o ../../Codes/CBB_CPF/timer_manage.c    
	
${OBJECTDIR}/_ext/2117827500/user_manage.o: ../../Codes/CBB_CPF/user_manage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2117827500 
	@${RM} ${OBJECTDIR}/_ext/2117827500/user_manage.o.d 
	@${RM} ${OBJECTDIR}/_ext/2117827500/user_manage.o.ok ${OBJECTDIR}/_ext/2117827500/user_manage.o.err 
	@${RM} ${OBJECTDIR}/_ext/2117827500/user_manage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2117827500/user_manage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/2117827500/user_manage.o.d" -o ${OBJECTDIR}/_ext/2117827500/user_manage.o ../../Codes/CBB_CPF/user_manage.c    
	
${OBJECTDIR}/_ext/1567105643/drv_uart.o: ../../Codes/drv/m2m_uart/drv_uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1567105643 
	@${RM} ${OBJECTDIR}/_ext/1567105643/drv_uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1567105643/drv_uart.o.ok ${OBJECTDIR}/_ext/1567105643/drv_uart.o.err 
	@${RM} ${OBJECTDIR}/_ext/1567105643/drv_uart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1567105643/drv_uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1567105643/drv_uart.o.d" -o ${OBJECTDIR}/_ext/1567105643/drv_uart.o ../../Codes/drv/m2m_uart/drv_uart.c    
	
${OBJECTDIR}/_ext/1474187024/drv_ad.o: ../../Codes/drv/ADC/drv_ad.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1474187024 
	@${RM} ${OBJECTDIR}/_ext/1474187024/drv_ad.o.d 
	@${RM} ${OBJECTDIR}/_ext/1474187024/drv_ad.o.ok ${OBJECTDIR}/_ext/1474187024/drv_ad.o.err 
	@${RM} ${OBJECTDIR}/_ext/1474187024/drv_ad.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1474187024/drv_ad.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1474187024/drv_ad.o.d" -o ${OBJECTDIR}/_ext/1474187024/drv_ad.o ../../Codes/drv/ADC/drv_ad.c    
	
${OBJECTDIR}/_ext/810413631/temp.o: ../../Codes/general/temp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/810413631 
	@${RM} ${OBJECTDIR}/_ext/810413631/temp.o.d 
	@${RM} ${OBJECTDIR}/_ext/810413631/temp.o.ok ${OBJECTDIR}/_ext/810413631/temp.o.err 
	@${RM} ${OBJECTDIR}/_ext/810413631/temp.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/810413631/temp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/810413631/temp.o.d" -o ${OBJECTDIR}/_ext/810413631/temp.o ../../Codes/general/temp.c    
	
${OBJECTDIR}/_ext/1223383274/comm.o: ../../Codes/bsp/comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1223383274 
	@${RM} ${OBJECTDIR}/_ext/1223383274/comm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1223383274/comm.o.ok ${OBJECTDIR}/_ext/1223383274/comm.o.err 
	@${RM} ${OBJECTDIR}/_ext/1223383274/comm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1223383274/comm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1223383274/comm.o.d" -o ${OBJECTDIR}/_ext/1223383274/comm.o ../../Codes/bsp/comm.c    
	
${OBJECTDIR}/_ext/2117827500/main.o: ../../Codes/CBB_CPF/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2117827500 
	@${RM} ${OBJECTDIR}/_ext/2117827500/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/2117827500/main.o.ok ${OBJECTDIR}/_ext/2117827500/main.o.err 
	@${RM} ${OBJECTDIR}/_ext/2117827500/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2117827500/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/2117827500/main.o.d" -o ${OBJECTDIR}/_ext/2117827500/main.o ../../Codes/CBB_CPF/main.c    
	
else
${OBJECTDIR}/_ext/1223384328/app_pcusart_protocol.o: ../../Codes/app/app_pcusart_protocol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1223384328 
	@${RM} ${OBJECTDIR}/_ext/1223384328/app_pcusart_protocol.o.d 
	@${RM} ${OBJECTDIR}/_ext/1223384328/app_pcusart_protocol.o.ok ${OBJECTDIR}/_ext/1223384328/app_pcusart_protocol.o.err 
	@${RM} ${OBJECTDIR}/_ext/1223384328/app_pcusart_protocol.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1223384328/app_pcusart_protocol.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1223384328/app_pcusart_protocol.o.d" -o ${OBJECTDIR}/_ext/1223384328/app_pcusart_protocol.o ../../Codes/app/app_pcusart_protocol.c    
	
${OBJECTDIR}/_ext/1223383274/app_autocheck.o: ../../Codes/bsp/app_autocheck.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1223383274 
	@${RM} ${OBJECTDIR}/_ext/1223383274/app_autocheck.o.d 
	@${RM} ${OBJECTDIR}/_ext/1223383274/app_autocheck.o.ok ${OBJECTDIR}/_ext/1223383274/app_autocheck.o.err 
	@${RM} ${OBJECTDIR}/_ext/1223383274/app_autocheck.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1223383274/app_autocheck.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1223383274/app_autocheck.o.d" -o ${OBJECTDIR}/_ext/1223383274/app_autocheck.o ../../Codes/bsp/app_autocheck.c    
	
${OBJECTDIR}/_ext/1223384328/app_ina219.o: ../../Codes/app/app_ina219.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1223384328 
	@${RM} ${OBJECTDIR}/_ext/1223384328/app_ina219.o.d 
	@${RM} ${OBJECTDIR}/_ext/1223384328/app_ina219.o.ok ${OBJECTDIR}/_ext/1223384328/app_ina219.o.err 
	@${RM} ${OBJECTDIR}/_ext/1223384328/app_ina219.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1223384328/app_ina219.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1223384328/app_ina219.o.d" -o ${OBJECTDIR}/_ext/1223384328/app_ina219.o ../../Codes/app/app_ina219.c    
	
${OBJECTDIR}/_ext/1223384328/app_irdausart_protocol.o: ../../Codes/app/app_irdausart_protocol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1223384328 
	@${RM} ${OBJECTDIR}/_ext/1223384328/app_irdausart_protocol.o.d 
	@${RM} ${OBJECTDIR}/_ext/1223384328/app_irdausart_protocol.o.ok ${OBJECTDIR}/_ext/1223384328/app_irdausart_protocol.o.err 
	@${RM} ${OBJECTDIR}/_ext/1223384328/app_irdausart_protocol.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1223384328/app_irdausart_protocol.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1223384328/app_irdausart_protocol.o.d" -o ${OBJECTDIR}/_ext/1223384328/app_irdausart_protocol.o ../../Codes/app/app_irdausart_protocol.c    
	
${OBJECTDIR}/_ext/1223383274/app_MeterUsart_protocol.o: ../../Codes/bsp/app_MeterUsart_protocol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1223383274 
	@${RM} ${OBJECTDIR}/_ext/1223383274/app_MeterUsart_protocol.o.d 
	@${RM} ${OBJECTDIR}/_ext/1223383274/app_MeterUsart_protocol.o.ok ${OBJECTDIR}/_ext/1223383274/app_MeterUsart_protocol.o.err 
	@${RM} ${OBJECTDIR}/_ext/1223383274/app_MeterUsart_protocol.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1223383274/app_MeterUsart_protocol.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1223383274/app_MeterUsart_protocol.o.d" -o ${OBJECTDIR}/_ext/1223383274/app_MeterUsart_protocol.o ../../Codes/bsp/app_MeterUsart_protocol.c    
	
${OBJECTDIR}/_ext/1623924188/bsp_mc308.o: ../../Codes/bsp/CBB_MCU_308_ForGprs.X/bsp_mc308.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1623924188 
	@${RM} ${OBJECTDIR}/_ext/1623924188/bsp_mc308.o.d 
	@${RM} ${OBJECTDIR}/_ext/1623924188/bsp_mc308.o.ok ${OBJECTDIR}/_ext/1623924188/bsp_mc308.o.err 
	@${RM} ${OBJECTDIR}/_ext/1623924188/bsp_mc308.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1623924188/bsp_mc308.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1623924188/bsp_mc308.o.d" -o ${OBJECTDIR}/_ext/1623924188/bsp_mc308.o ../../Codes/bsp/CBB_MCU_308_ForGprs.X/bsp_mc308.c    
	
${OBJECTDIR}/_ext/1623924188/lib_mc308.o: ../../Codes/bsp/CBB_MCU_308_ForGprs.X/lib_mc308.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1623924188 
	@${RM} ${OBJECTDIR}/_ext/1623924188/lib_mc308.o.d 
	@${RM} ${OBJECTDIR}/_ext/1623924188/lib_mc308.o.ok ${OBJECTDIR}/_ext/1623924188/lib_mc308.o.err 
	@${RM} ${OBJECTDIR}/_ext/1623924188/lib_mc308.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1623924188/lib_mc308.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1623924188/lib_mc308.o.d" -o ${OBJECTDIR}/_ext/1623924188/lib_mc308.o ../../Codes/bsp/CBB_MCU_308_ForGprs.X/lib_mc308.c    
	
${OBJECTDIR}/_ext/2117827500/task_manage.o: ../../Codes/CBB_CPF/task_manage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2117827500 
	@${RM} ${OBJECTDIR}/_ext/2117827500/task_manage.o.d 
	@${RM} ${OBJECTDIR}/_ext/2117827500/task_manage.o.ok ${OBJECTDIR}/_ext/2117827500/task_manage.o.err 
	@${RM} ${OBJECTDIR}/_ext/2117827500/task_manage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2117827500/task_manage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/2117827500/task_manage.o.d" -o ${OBJECTDIR}/_ext/2117827500/task_manage.o ../../Codes/CBB_CPF/task_manage.c    
	
${OBJECTDIR}/_ext/2117827500/timer_manage.o: ../../Codes/CBB_CPF/timer_manage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2117827500 
	@${RM} ${OBJECTDIR}/_ext/2117827500/timer_manage.o.d 
	@${RM} ${OBJECTDIR}/_ext/2117827500/timer_manage.o.ok ${OBJECTDIR}/_ext/2117827500/timer_manage.o.err 
	@${RM} ${OBJECTDIR}/_ext/2117827500/timer_manage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2117827500/timer_manage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/2117827500/timer_manage.o.d" -o ${OBJECTDIR}/_ext/2117827500/timer_manage.o ../../Codes/CBB_CPF/timer_manage.c    
	
${OBJECTDIR}/_ext/2117827500/user_manage.o: ../../Codes/CBB_CPF/user_manage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2117827500 
	@${RM} ${OBJECTDIR}/_ext/2117827500/user_manage.o.d 
	@${RM} ${OBJECTDIR}/_ext/2117827500/user_manage.o.ok ${OBJECTDIR}/_ext/2117827500/user_manage.o.err 
	@${RM} ${OBJECTDIR}/_ext/2117827500/user_manage.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2117827500/user_manage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/2117827500/user_manage.o.d" -o ${OBJECTDIR}/_ext/2117827500/user_manage.o ../../Codes/CBB_CPF/user_manage.c    
	
${OBJECTDIR}/_ext/1567105643/drv_uart.o: ../../Codes/drv/m2m_uart/drv_uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1567105643 
	@${RM} ${OBJECTDIR}/_ext/1567105643/drv_uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1567105643/drv_uart.o.ok ${OBJECTDIR}/_ext/1567105643/drv_uart.o.err 
	@${RM} ${OBJECTDIR}/_ext/1567105643/drv_uart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1567105643/drv_uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1567105643/drv_uart.o.d" -o ${OBJECTDIR}/_ext/1567105643/drv_uart.o ../../Codes/drv/m2m_uart/drv_uart.c    
	
${OBJECTDIR}/_ext/1474187024/drv_ad.o: ../../Codes/drv/ADC/drv_ad.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1474187024 
	@${RM} ${OBJECTDIR}/_ext/1474187024/drv_ad.o.d 
	@${RM} ${OBJECTDIR}/_ext/1474187024/drv_ad.o.ok ${OBJECTDIR}/_ext/1474187024/drv_ad.o.err 
	@${RM} ${OBJECTDIR}/_ext/1474187024/drv_ad.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1474187024/drv_ad.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1474187024/drv_ad.o.d" -o ${OBJECTDIR}/_ext/1474187024/drv_ad.o ../../Codes/drv/ADC/drv_ad.c    
	
${OBJECTDIR}/_ext/810413631/temp.o: ../../Codes/general/temp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/810413631 
	@${RM} ${OBJECTDIR}/_ext/810413631/temp.o.d 
	@${RM} ${OBJECTDIR}/_ext/810413631/temp.o.ok ${OBJECTDIR}/_ext/810413631/temp.o.err 
	@${RM} ${OBJECTDIR}/_ext/810413631/temp.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/810413631/temp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/810413631/temp.o.d" -o ${OBJECTDIR}/_ext/810413631/temp.o ../../Codes/general/temp.c    
	
${OBJECTDIR}/_ext/1223383274/comm.o: ../../Codes/bsp/comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1223383274 
	@${RM} ${OBJECTDIR}/_ext/1223383274/comm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1223383274/comm.o.ok ${OBJECTDIR}/_ext/1223383274/comm.o.err 
	@${RM} ${OBJECTDIR}/_ext/1223383274/comm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1223383274/comm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/1223383274/comm.o.d" -o ${OBJECTDIR}/_ext/1223383274/comm.o ../../Codes/bsp/comm.c    
	
${OBJECTDIR}/_ext/2117827500/main.o: ../../Codes/CBB_CPF/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2117827500 
	@${RM} ${OBJECTDIR}/_ext/2117827500/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/2117827500/main.o.ok ${OBJECTDIR}/_ext/2117827500/main.o.err 
	@${RM} ${OBJECTDIR}/_ext/2117827500/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2117827500/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -DDEF_HALL_ON_BOARD -DDEF_TEMP_PRESS -I"../../Codes/app" -I"../../Codes/board" -I"../../Codes/bsp" -I"../../Codes/CBB_CPF" -I"../../Codes/drv" -I"../../Codes/general" -I"../../Codes/drv/lcd" -I"../../simple/ProjTest" -I"../../Codes/bsp/CBB_MCU_308_ForGprs.X" -I"../../Codes/drv/m2m_uart" -I"../../Codes/drv/ADC" -I"../../Codes/app/PowerManage" -MMD -MF "${OBJECTDIR}/_ext/2117827500/main.o.d" -o ${OBJECTDIR}/_ext/2117827500/main.o ../../Codes/CBB_CPF/main.c    
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/AutoTest.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=elf -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -o dist/${CND_CONF}/${IMAGE_TYPE}/AutoTest.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -Wl,--defsym=__MPLAB_BUILD=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1
else
dist/${CND_CONF}/${IMAGE_TYPE}/AutoTest.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=elf -mcpu=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/AutoTest.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -Wl,--defsym=__MPLAB_BUILD=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)
	${MP_CC_DIR}\\pic30-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/AutoTest.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -omf=elf
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/128K_Hall_Onboard
	${RM} -r dist/128K_Hall_Onboard

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif

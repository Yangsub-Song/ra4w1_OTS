################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../qe_gen/ble/r_ble_gapc.c \
../qe_gen/ble/r_ble_gaps.c \
../qe_gen/ble/r_ble_gats.c \
../qe_gen/ble/r_ble_hrs.c \
../qe_gen/ble/r_ble_lss.c \
../qe_gen/ble/r_ble_ots.c \
../qe_gen/ble/r_ble_ots_record.c 

C_DEPS += \
./qe_gen/ble/r_ble_gapc.d \
./qe_gen/ble/r_ble_gaps.d \
./qe_gen/ble/r_ble_gats.d \
./qe_gen/ble/r_ble_hrs.d \
./qe_gen/ble/r_ble_lss.d \
./qe_gen/ble/r_ble_ots.d \
./qe_gen/ble/r_ble_ots_record.d 

OBJS += \
./qe_gen/ble/r_ble_gapc.o \
./qe_gen/ble/r_ble_gaps.o \
./qe_gen/ble/r_ble_gats.o \
./qe_gen/ble/r_ble_hrs.o \
./qe_gen/ble/r_ble_lss.o \
./qe_gen/ble/r_ble_ots.o \
./qe_gen/ble/r_ble_ots_record.o 


# Each subdirectory must supply rules for building sources it contributes
qe_gen/ble/%.o: ../qe_gen/ble/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -D_RENESAS_RA_ -D_RA_CORE=CM4 -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\src" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\ra\fsp\inc" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\ra\fsp\inc\api" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\ra\fsp\inc\instances" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\ra\arm\CMSIS_5\CMSIS\Core\Include" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\ra_gen" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\ra_cfg\fsp_cfg" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\ra_cfg\fsp_cfg\bsp" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\qe_gen" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\qe_gen\ble" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\src\app_lib\cli\console" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\src\app_lib\cmd" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\src\app_lib" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\src\app_lib\cli\console\ra_family" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\src\app_lib\cli" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\src\app_lib\logger" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<"
	@echo 'Finished building: $<'
	@echo ' '



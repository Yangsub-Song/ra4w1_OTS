################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/r_ioport/r_ioport.c 

C_DEPS += \
./ra/fsp/src/r_ioport/r_ioport.d 

OBJS += \
./ra/fsp/src/r_ioport/r_ioport.o 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/r_ioport/%.o: ../ra/fsp/src/r_ioport/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -D_RENESAS_RA_ -D_RA_CORE=CM4 -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\src" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\ra\fsp\inc" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\ra\fsp\inc\api" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\ra\fsp\inc\instances" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\ra\arm\CMSIS_5\CMSIS\Core\Include" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\ra_gen" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\ra_cfg\fsp_cfg" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\ra_cfg\fsp_cfg\bsp" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\qe_gen" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\qe_gen\ble" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\src\app_lib\cli\console" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\src\app_lib\cmd" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\src\app_lib" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\src\app_lib\cli\console\ra_family" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\src\app_lib\cli" -I"C:\Users\USER\e2_studio\workspace3\ble_baremetal_ek_ra4w1\src\app_lib\logger" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<"
	@echo 'Finished building: $<'
	@echo ' '



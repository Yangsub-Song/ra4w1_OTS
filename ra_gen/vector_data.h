/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (8)
#endif
/* ISR prototypes */
void r_icu_isr(void);
void sci_uart_rxi_isr(void);
void sci_uart_txi_isr(void);
void sci_uart_tei_isr(void);
void sci_uart_eri_isr(void);
void gpt_counter_overflow_isr(void);
void agt_int_isr(void);

/* Vector table allocations */
#define VECTOR_NUMBER_ICU_IRQ4 ((IRQn_Type) 0) /* ICU IRQ4 (External pin interrupt 4) */
#define ICU_IRQ4_IRQn          ((IRQn_Type) 0) /* ICU IRQ4 (External pin interrupt 4) */
#define VECTOR_NUMBER_SCI4_RXI ((IRQn_Type) 1) /* SCI4 RXI (Received data full) */
#define SCI4_RXI_IRQn          ((IRQn_Type) 1) /* SCI4 RXI (Received data full) */
#define VECTOR_NUMBER_SCI4_TXI ((IRQn_Type) 2) /* SCI4 TXI (Transmit data empty) */
#define SCI4_TXI_IRQn          ((IRQn_Type) 2) /* SCI4 TXI (Transmit data empty) */
#define VECTOR_NUMBER_SCI4_TEI ((IRQn_Type) 3) /* SCI4 TEI (Transmit end) */
#define SCI4_TEI_IRQn          ((IRQn_Type) 3) /* SCI4 TEI (Transmit end) */
#define VECTOR_NUMBER_SCI4_ERI ((IRQn_Type) 4) /* SCI4 ERI (Receive error) */
#define SCI4_ERI_IRQn          ((IRQn_Type) 4) /* SCI4 ERI (Receive error) */
#define VECTOR_NUMBER_ICU_IRQ8 ((IRQn_Type) 5) /* BLEIRQ (Only for BLE middleware use) */
#define ICU_IRQ8_IRQn          ((IRQn_Type) 5) /* BLEIRQ (Only for BLE middleware use) */
#define VECTOR_NUMBER_GPT1_COUNTER_OVERFLOW ((IRQn_Type) 6) /* GPT1 COUNTER OVERFLOW (Overflow) */
#define GPT1_COUNTER_OVERFLOW_IRQn          ((IRQn_Type) 6) /* GPT1 COUNTER OVERFLOW (Overflow) */
#define VECTOR_NUMBER_AGT0_INT ((IRQn_Type) 7) /* AGT0 INT (AGT interrupt) */
#define AGT0_INT_IRQn          ((IRQn_Type) 7) /* AGT0 INT (AGT interrupt) */
#endif /* VECTOR_DATA_H */

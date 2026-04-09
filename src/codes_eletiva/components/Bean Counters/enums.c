#ifndef BEAN_ENUMS
#define BEAN_ENUMS

// Estados de carga do movimento do pinguim
typedef enum
{
    NAO_CARREGA,
    CARREGA1,
    CARREGA2,
    CARREGA3,
    CARREGA4,
    CARREGA5,
    CARREGA6

} Estados_pinguim_carga;

// Estados de movimento do pinguim
typedef enum 
{
    PARADO,
    MOVENDO
} Estados_pinguim_movimento;



// Tipos de carga
typedef enum
{
    GRAOS,
    PEIXE,
    BIGORNA,
    VASO
} Enum_carga;

#endif
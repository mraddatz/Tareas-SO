 # Tarea 2 - parte 2
 
 ## Autores:
  - Matias Correa (15634183)
  - Martin Raddatz (15634167)
 
 ## Supuestos y decisiones de diseno
Primero que nada, se decidio no utilizar los tres bits extras de la page table, se utilizo solo uno de ellos, llamado `obsol_bit`, que indica si una entrada esta obsoleta, i.e. el frame al que apunta ya no tiene la informacion que le corresponde a la entrada.

Para modelar cada entidad se crearon estructuras, que contienen arreglos de estructuras que corresponden a las entradas de casa una. Por ejemplo, la TLB tiene un arreglo de estructuras `TLB_Entry`. Se distingue entre `PageTable` y `PageDirectory`. Esta ultima son las tablas intermedias, que apuntan a otra tabla. La `PageTable` apunta a frames de memoria.

El archivo `structs` contiene todas las estructuras creadas y sus funciones asociadas. El archivo `optimize` es el que contiene el algoritmo para determinar el tamano de cada nivel de paginacion. Finalmente, `p2` es el archivo principal.
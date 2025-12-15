estudiantes
Ana Faisury Garcia Anizares
Dilan Maurucio Perea Quiroja

 Estructuras de Datos 
 proyecto final: sistemas logistico

El proyecto se basa en un diseño híbrido para asegurar la eficiencia en la entrada (recepción) y salida (despacho) de mercancías:

1. un (Inventario de Lotes)Se utiliza un Árbol AVL para almacenar y gestionar los diferentes lotes de mercancía que ingresan al puerto.
*la Clave de Ordenamiento del AVL es la Fecha de Vencimiento.
*como Prioridad: Esta estructura garantiza que, al buscar el lote a despachar, se acceda inmediatamente al nodo de la izquierda (el que tiene la fecha de vencimiento más antigua), implementando la política general de inventario FIFO para la mercancía.
*El AVL asegura que la inserción de nuevos lotes (recepción de mercancía) sea siempre rápida manteniendo el árbol balanceado.
2. en Cola FIFO (Pedidos por Lote)Cada nodo del Árbol AVL (es decir, cada lote de producto) contiene internamente una estructura de Cola (FIFO).
* su Funciónalidad en Esta cola, almacena los pedidos de despacho (encolarPedido) que se hacen contra ese lote específico. *Garantiza que, si varios destinos solicitan unidades del mismo lote prioritario, los pedidos se atiendan en el estricto orden en que fueron recibidos (Primer Pedido Entrado, Primer Pedido Atendido).
  ofrece un menú completo para gestionar el ciclo de vida de la mercancía, desde la entrada hasta el despacho y la cancelación:FunciónEstructura PrincipalDescripción1. Recepción de MercancíaAVL (Inserción)Registra un nuevo lote en el inventario, usando la fecha de vencimiento como clave. Se realizan rotaciones para mantener el balance del AVL.
  * Registrar PedidoAVL (Búsqueda) + Cola FIFO (Inserción)
  * Busca automáticamente el lote más antiguo (más a la izquierda).
  * Encola el pedido al final de la cola de ese lote, y reduce el stock_total.
  * Baja de ProductoAVL (Eliminación)Permite eliminar un lote completo (por fecha de vencimiento). Automáticamente libera la memoria de todos los pedidos pendientes asociados a ese lote.
  * Cancelar PedidoCola FIFO (Eliminación intermedia)Permite buscar y eliminar un pedido específico de la cola del lote correspondiente. El stock es restablecido al stock_total del lote.
  * Reporte de EstadoAVL (Recorrido In-Order)Muestra la lista de todos los lotes de inventario ordenados por prioridad de despacho (fecha de vencimiento más antigua primero).Uso y Reglas de NegocioPrioridad de Despacho (FIFO General): Todos los pedidos deben ser procesados contra el lote con la fecha de vencimiento más cercana (el nodo más a la izquierda del AVL).Prioridad de Pedido  Dentro de cada lote, los pedidos se atienden en orden de llegada.
  Destinos Iniciales: El sistema precarga pedidos separados para los destinos Guapi, Timbiquí, Nuqui, y Bahía Solano contra el lote de fecha más antigua (Leche, 20251110) para demostrar la funcionalidad de la cola FIFO con múltiples entradas individuales.
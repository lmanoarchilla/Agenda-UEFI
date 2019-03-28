/** 
		Archivo de cabecera para las funciones de la aplicación UEFI Agenda
		
	  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/
#ifndef Agenda_h
#define Agenda_h

//
// Estructura de los nodos de la agenda
//
typedef struct ENTRADA{
	CHAR16 Nombre[30];
	CHAR16  Telefono[10];
	struct ENTRADA *siguiente;
}ENTRADA;
/**
  Esta función muestra los datos nombre y teléfono una entrada.

  @param  Entrada        		Entrada de la agenda.
**/
EFI_INPUT_KEY ImprimeMenu();
/**
  Esta función muestra una entrada de la agenda
  @param  Primero        		Puntero al primer elemento de la lista.   
**/
VOID ImprimeEntrada(ENTRADA *Entrada);
/**
  Esta función lee lo introducido por el teclado y lo guarda en una variable de tipo string.

  @param  Buff        		String que recoge la entrada del teclado.
  @param  TamMax 			tamaño máximo del string.  
**/
VOID ObtenerString(CHAR16 *Buff, UINTN TamMax);
/**
  Esta función muestra todas las entradas a la agenda
  @param  Primero        		Puntero al primer elemento de la lista.   
**/
VOID Mostrar(ENTRADA *Primero);
/**
  Esta función busca una entrada en la agenda y permite modificar el valor teléfono.

  @param  Primero        		Puntero al primer elemento de la lista.

**/
VOID Modificar(ENTRADA *Primero);
/**
  Esta función crea una entrada en la agenda e inserta un nodo en la lista enlazada.

  @param  Primero        	Puntero al primer elemento de la lista.

  @retval ENTRADA           Devuelve el primer puntero de la lista.  

**/
ENTRADA *Nuevo(ENTRADA *Primero);
/**
  Esta función borra una entrada en la agenda e inserta un nodo en la lista enlazada.

  @param  Primero        	Puntero al primer elemento de la lista.

  @retval ENTRADA           Devuelve el primer puntero de la lista.  
  
**/
ENTRADA *Borrar(ENTRADA *Primero);

#endif
/** @file
  Esta es una aplicación UEFI sencilla de tipo agenda. 
  
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Agenda.h>

/**  
	Función principal de la aplicación

  @param[in] HandleImagen    Handle asignado por el firmware para la imagen UEFI. 
  @param[in] TablaSistema    Puntero a la tabla del sistema UEFI
  
  @retval EFI_SUCCESS       El punto de entrada ha sido ejecutado satisfactoriamente
  @retval other             Errores ocurridos durante la ejecución del punto de entrada

**/
EFI_STATUS EFIAPI UefiMain (IN EFI_HANDLE HandleImagen, IN EFI_SYSTEM_TABLE  *TablaSistema){

    EFI_INPUT_KEY	TeclaPulsada;
    BOOLEAN	Salir=FALSE;
    ENTRADA	*Primero = NULL;   		

	do{		
		gST->ConOut->ClearScreen(gST->ConOut);
		TeclaPulsada = ImprimeMenu();	
		//Evento de tipo timer, espera que se presione una tecla para continuar la ejecución
		switch(TeclaPulsada.ScanCode){			
			case SCAN_F1 :	
				gST->ConOut->ClearScreen(gST->ConOut);
				Print(L"F1 has been selected\n\n");	
				Primero = Nuevo(Primero);	
				break;

			case SCAN_F2 :
				gST->ConOut->ClearScreen(gST->ConOut);
				Print(L"F2 has been selected\n\n");
				Mostrar(Primero);								
				break;

			case SCAN_F3 :				
			    gST->ConOut->ClearScreen(gST->ConOut);
				Print(L"F3 has been selected\n\n");
				Modificar(Primero);
				break;	

			case SCAN_F4:
				gST->ConOut->ClearScreen(gST->ConOut);
				Print(L"F4 has been selected\n\n");					
				Primero = Borrar(Primero);				
				break;			

			case SCAN_F5 :
			    Print(L"Exit\n\n");
				Salir=TRUE;
				break;

			default :
				Print(L"Selected option is not avaible\n\n");							
		}			
	}while(Salir==FALSE);	
	
	
	return EFI_SUCCESS;
}

EFI_INPUT_KEY ImprimeMenu(){
	UINTN  IndiceEventos;
    EFI_INPUT_KEY	TeclaPulsada;

	Print(L"******************************************\n");
	Print(L"*   		    PHONE BOOK    	 	 	 *\n");
	Print(L"******************************************\n");
	Print(L"	F1. Add Telephone Record\n");
	Print(L"	F2. Show Records\n");
	Print(L"	F3. Modify Record \n");
	Print(L"	F4. Delete Record\n");
	Print(L"	F5. Exit\n");
	gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &IndiceEventos);
	gST->ConIn->ReadKeyStroke(gST->ConIn, &TeclaPulsada);

	return TeclaPulsada;

}

VOID ImprimeEntrada(ENTRADA *Entrada){
	UINTN IndiceEventos;
    EFI_INPUT_KEY TeclaPulsada;

	Print(L"**************************************************\n");			
	Print(L"* Phone Number: %s\n",Entrada->Telefono);
	Print(L"* Name: %s\n",Entrada->Nombre);
	Print(L"**************************************************\n");
	gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &IndiceEventos);
	gST->ConIn->ReadKeyStroke (gST->ConIn, &TeclaPulsada);

}

VOID ObtenerString(CHAR16 *Buff, UINTN TamMax){
	UINTN  IndiceEventos;
    EFI_INPUT_KEY	TeclaPulsada;
    UINTN cont = 0;
    do {
		gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &IndiceEventos);
		gST->ConIn->ReadKeyStroke (gST->ConIn, &TeclaPulsada);
		Print(L"%c", TeclaPulsada.UnicodeChar);
		if(TeclaPulsada.UnicodeChar == CHAR_LINEFEED || TeclaPulsada.UnicodeChar == CHAR_CARRIAGE_RETURN){
			break;
		}
		else if (TeclaPulsada.UnicodeChar!=CHAR_BACKSPACE){
				Buff[cont]=TeclaPulsada.UnicodeChar;	
				cont++;
			}else cont--;   
 	
	}while(cont<=(TamMax-2)); 
	Buff[cont]=L'\0';
}

VOID Mostrar(ENTRADA *Primero){
	UINTN  IndiceEventos;
    EFI_INPUT_KEY	TeclaPulsada;
	ENTRADA *Auxiliar = NULL;
	Auxiliar = Primero;

	while(!(Auxiliar==NULL)){
		ImprimeEntrada(Auxiliar);	
		Auxiliar = Auxiliar->siguiente;		
	}

	Print(L"No more Records\n");	
	gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &IndiceEventos);
	gST->ConIn->ReadKeyStroke (gST->ConIn, &TeclaPulsada);				
}


ENTRADA *Nuevo(ENTRADA *Primero){	
	EFI_STATUS efiStatus;
	ENTRADA *nuevaEntrada = NULL;
	UINTN  IndiceEventos;
	efiStatus = gBS->AllocatePool(EfiLoaderData, sizeof(ENTRADA), (VOID *) &nuevaEntrada);	
	if (EFI_ERROR (efiStatus)) {
		Print(L"Error: The pool requested could not be allocated.\n");
		Print(L"Program Exit\n");
		gBS->Stall(3 * 1000 * 1000);
		return NULL;
	}

	Print(L"\nEnter Name: ");
	ObtenerString(nuevaEntrada->Nombre,30);					
	Print(L"\nEnter Phone Number: ");
	ObtenerString(nuevaEntrada->Telefono,10);
	Print(L"\nPress any Key...");
	gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &IndiceEventos);

	//Print(L"\nEl teléfono introducido es: %s\n",nuevaEntrada->Telefono); 

	nuevaEntrada->siguiente = NULL ;
	ENTRADA *Auxiliarf = NULL;
	Auxiliarf = Primero;

	if (Primero==NULL){		 
		Primero = nuevaEntrada;
	}else{ 
		while (Auxiliarf->siguiente!=NULL) Auxiliarf = Auxiliarf->siguiente;	        		
		Auxiliarf->siguiente = nuevaEntrada;
	}	
	return Primero;
}

VOID Modificar(ENTRADA *Primero){
	UINTN  IndiceEventos;
    EFI_INPUT_KEY	TeclaPulsada;
	ENTRADA *Auxiliar = Primero;
	CHAR16 Nombre[30];

	Print(L"Please Enter The Name: ");
	ObtenerString(Nombre,30);
	Print(L"\nRecord found:  %s\n",Nombre);
	gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &IndiceEventos);
	gST->ConIn->ReadKeyStroke (gST->ConIn, &TeclaPulsada);

	UINTN i=0;
	while(!(Auxiliar==NULL)){					
		i=0;
		while (!(Nombre[i] ==L'\0') && (Nombre[i] == Auxiliar->Nombre[i])) i++;
		if ((Nombre[i] - Auxiliar->Nombre[i])== 0){
			Print(L"\nPlease Enter The New Phone Number: \n");							
			ObtenerString(Auxiliar->Telefono,10);
			Print(L"\nInserted Phone Number: %s\n",Auxiliar->Telefono);
			Print(L"\nPress any Key...");
			gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &IndiceEventos);
			gST->ConIn->ReadKeyStroke (gST->ConIn, &TeclaPulsada);
			break;
		}Auxiliar = Auxiliar->siguiente;								
	}
}

ENTRADA *Borrar(ENTRADA *Primero){
	UINTN  IndiceEventos;
    EFI_INPUT_KEY	TeclaPulsada;
    EFI_STATUS efiStatus;

	ENTRADA *Auxiliar = Primero;
	CHAR16 Nombre[30];
	
	ENTRADA *Borrar=NULL; 

	Print(L"Please Enter The Name: ");	
	ObtenerString(Nombre,30);
	Print(L"\nRecord found: %s\n",Nombre);
	UINTN i;
	if (Primero!=NULL){
		Print(L"Starting the search\n");
		i=0;
		while (!(Nombre[i] ==L'\0') && (Nombre[i] == Primero->Nombre[i])) i++;
		if ((Nombre[i] - Primero->Nombre[i])== 0){	
			Auxiliar = Primero->siguiente;			
			Primero = Auxiliar;
		}else{
			while((Auxiliar!=NULL)&&(Auxiliar->siguiente->Nombre)){					
				i=0;
				while (!(Nombre[i] ==L'\0') && (Nombre[i] == Auxiliar->siguiente->Nombre[i])) i++;							
				if ((Nombre[i] - Auxiliar->siguiente->Nombre[i])== 0){
					Borrar= Auxiliar->siguiente;	
					Auxiliar->siguiente = Auxiliar ->siguiente->siguiente;
					efiStatus = gBS->FreePool(Borrar);					
					if (EFI_ERROR (efiStatus)) {
		    			Print(L"Error: The pool requested could not be deleted.\n");
		    			gBS->Stall(3 * 1000 * 1000);
		    			return Primero;
		    		}	
		    		Print(L"Record deleted\n");
					gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &IndiceEventos);
					gST->ConIn->ReadKeyStroke (gST->ConIn, &TeclaPulsada);							
					break;									
				}else Auxiliar = Auxiliar->siguiente;
			}
		}
	}
	gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &IndiceEventos);
	gST->ConIn->ReadKeyStroke (gST->ConIn, &TeclaPulsada);
	return Primero;
}
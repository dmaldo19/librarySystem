#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <windows.h>

#define MAX 80
#define VALOR_CENTINELA -1

/* Estructuras */
struct libro {
    int codigo; //Codigo del libro
    char titulo[MAX]; //Titulo del libro
    char autor[MAX]; //Autor del libro
	char editorial[MAX]; //Editorial
	char fecha[8]; //Fecha de publicación
	int anyo;
	int cantidad;
	float precio;
};

typedef struct libro Producto;

/* prototipo de las opciones del Menú */
void menuPrincipal();
void menuIngresar();
void menuBuscarcodigo();
void menuEliminar();
void menuMostrar();
void menuModificar();

//Prototipo funciones de diseño
void gotoxy(int, int);
void cuadro(int xs, int ys, int xi, int yi);
void cargando();
void desactivarMaximizar();

//prototipo funciones que interaccionan con el archivo
Producto *obtenerInventario(int *n);
char existeProducto(int codigoLibro, Producto *libro);
char ingresarProducto(Producto libro);
char eliminarProducto(int codigoLibro);
char modificarProducto(Producto libro);
char guardarReporte(); /*txt*/

/* Función de lectura de cadenas */
int leecad(char *cad, int n);

/* Titular del programa */
void tituloPrincipal();
char linea[MAX];

int main()//funcion main
{
    system("mode con: cols=100 lines=25");//funcion para darle tamaño a la ventana que se abre
    system("COLOR D7"); //funcion para dar color al fondo y a las letras
	desactivarMaximizar(); //funcion que impide maximizar la ventana
	menuPrincipal(); //funcion del menu principal

	return 0;
}

void menuPrincipal()
{
	char repite = 1;
	char salida[MAX];
	int opcion = -1;
	do { //estructura de control repetitiva hacer mientras

		system("cls");
		tituloPrincipal();//funcion para mostrar el encabezado del programa

		gotoxy(43,5);printf("MENU PRINCIPAL");
		gotoxy(2,6);printf("[1] Ingresar nuevo libro");
		gotoxy(2,7);printf("[2] Eliminar un libro");
		gotoxy(2,8);printf("[3] Modificar un libro");
		gotoxy(2,9);printf("[4] Mostrar listado de productos (generar reporte)");
		gotoxy(2,10);printf("[5] Salir");
		gotoxy(32,20);printf("Ingrese la opcion a realizar -> [ ]\b\b");

		/* Lectura segura de un entero */
		leecad(linea, MAX);
		sscanf(linea, "%d", &opcion);//Leer la seleccion del usuario

		switch (opcion) { //estructura de control selectiva multiple

			case 1:
				menuIngresar();
				break;

			case 2:
				menuEliminar();
				break;

			case 3:
				menuModificar();
				break;

			case 4:
				menuBuscarClave();
				break;

			case 5:
			    system("cls");
			    tituloPrincipal();
			    gotoxy(28,20);printf("Esta seguro que desea salir? [SI/NO]: ");
                leecad(salida, MAX);

                if ((strcmp(salida, "SI") == 0 || strcmp(salida, "si") == 0)) { //si la respuesta es si, cierra el programa
                    repite = 0;
                }

				break;
		}

	} while (repite);
}

void menuIngresar()
{//funcion que permite ingresar los valores de un nuevo producto
	Producto libro; //se crea la variable libro de tipo producto
	int codigoLibro = 0;
	char repite = 1;
	char respuesta[MAX];

	do {
		system("cls");
        tituloPrincipal();
		gotoxy(37,5);printf("Ingresar libros al almacen");

		codigoLibro = 0;
		gotoxy(2,6);printf("Ingresa el codigo del libro: ");
		leecad(linea, MAX);
		sscanf(linea, "%d", &codigoLibro);

		while(codigoLibro <= 0){ //validar si el codigo que ingreso es correcto
            gotoxy(2,6);printf("Ingrese un codigo valido, debe ser un codigo numerico mayor a 0: ");
            leecad(linea, MAX);
            sscanf(linea, "%d", &codigoLibro);
		}

		if (!existeProducto(codigoLibro, &libro)) { //si no existe un miembro con el codigo que ingreso el usuario, entonces procede a crearlo
			libro.codigo = codigoLibro; //le asigna el codigo que ingreso previamente

			gotoxy(2,7);printf("Ingrese el Titulo: ");
			leecad(libro.titulo, MAX);//ingresa el titulo

			gotoxy(2,8);printf("Ingrese el Autor: ");
			leecad(libro.autor, MAX);//ingresa el autor

			gotoxy(2,9);printf("Ingrese la Editorial: ");
			leecad(libro.editorial, MAX); //ingresa la editorial

			gotoxy(2,10);printf("Fecha de publicacion dd/mm/aa: ");
			leecad(libro.fecha, MAX); //ingresa la fecha de publicacion

			libro.anyo = 0;
			gotoxy(2,11);printf("Ingrese el Anyo de publicacion: ");
			leecad(linea, MAX); //ingresa el anyo de publicacion
			sscanf(linea, "%d", &libro.anyo);
			while(libro.anyo < 1||libro.anyo > 10000){ //estructura de control repetitiva mientras para ver si el anyo ingresado es valido
                gotoxy(2,11);printf("                                                  ");
                gotoxy(2,11);printf("Ingrese un anyo valido: ");
                leecad(linea, MAX);
                sscanf(linea, "%d", &libro.anyo);
			}

			libro.cantidad = 0;
			gotoxy(2,12);printf("Cantidad de libros a ingresar: ");//ingresa la cantidad de libros
			leecad(linea, MAX);
			sscanf(linea, "%d", &libro.cantidad);
			while(libro.cantidad < 1|| libro.cantidad > 1000000){ //estructura de control repetitiva mientras para confirmar que la cantidad ingresada es valida
                gotoxy(2,12);printf("                                                  ");
                gotoxy(2,12);printf("Ingrese una cantidad valida: ");
                leecad(linea, MAX);
                sscanf(linea, "%d", &libro.cantidad);
			}

			libro.precio = 0.0;
			gotoxy(2,13);printf("Precio del libro($): "); //ingresa el precio del libro
			leecad(linea, MAX);
			sscanf(linea, "%f", &libro.precio);

			while(libro.precio < 1){//estructura de control repetitiva mientras para verificar si el precio es valido
                gotoxy(2,13);printf("                                                  ");
                gotoxy(2,13);printf("Ingrese un precio de libro valido: ");
                leecad(linea, MAX);
                sscanf(linea, "%f", &libro.precio);
			}


			/* Se inserta el producto en el archivo */
			if (ingresarProducto(libro)) {
				gotoxy(32,15);printf("El libro fue ingresado correctamente");

			} else {
				gotoxy(23,15);printf("ERROR... El libro no pudo ser ingresado correctamente");
				gotoxy(36,16);printf("Favor de volver a intentarlo");
			}
		} else {
			/* El libro ya existe */
			gotoxy(2,7);printf("El libro de codigo %d ya existe.", codigoLibro);
			gotoxy(2,8);printf("ERROR... No es posible ingresar este producto debido que, ya existe un libro con ese código.");
		}

		gotoxy(28,20);printf("Desea seguir ingresando productos? [SI/NO]: ");
		leecad(respuesta, MAX);

		if (!(strcmp(respuesta, "SI") == 0 || strcmp(respuesta, "si") == 0)) { //si la respuesta no es si, se sale del hacer mientras
			repite = 0;
		}

	} while (repite);
}


void menuBuscarClave()
{//funcion que permite buscar los libros que hay en inventario
	Producto libro; //se crea la variable libro de tipo de dato complejo producto
	int codigoLibro;
	char repite = 1;
	char respuesta[MAX];

	do { //estructura de control repetitiva hacer mientras
		system("cls");
		tituloPrincipal();
		gotoxy(7,23);printf("Como codigos predeterminados para la prueba del programa tenemos desde el 1 hasta el 8");
		gotoxy(38,5);printf("Buscar libro por codigo");
		gotoxy(40,6);printf("Codigo del libro: ");
		leecad(linea, MAX);
		sscanf(linea, "%d", &codigoLibro);

		if (existeProducto(codigoLibro, &libro)) { //si encuentra un libro con el codigo ingresado por el usuario, muestras sus datos

			gotoxy(2,7);printf("Codigo del producto: %d", libro.codigo);
			gotoxy(2,8);printf("Titulo: %s", libro.titulo);
			gotoxy(2,9);printf("Autor: %s", libro.autor);
			gotoxy(2,10);printf("Editorial: %s", libro.editorial);
			gotoxy(2,11);printf("Fecha de publicacion: %s", libro.fecha);
			gotoxy(2,12);printf("Anyo: %d", libro.anyo);
			gotoxy(2,13);printf("Cantidad: %d\n", libro.cantidad);
			gotoxy(2,14);printf("Precio del libro: %.1f $", libro.precio);


		} else { //si no lo encuentra, dice que no existe
            //El producto no existe
			gotoxy(2,7);printf("El libro de codigo %d no existe.", codigoLibro);
		}

		gotoxy(27,20);printf("Desea continuar buscando algun libro? [SI/NO]: ");
		leecad(respuesta, MAX);

		if (!(strcmp(respuesta, "SI") == 0 || strcmp(respuesta, "si") == 0)) {
			repite = 0;
		}

	} while (repite);
}

void menuEliminar()
{ //funcion que permite eliminar productos del inventario
	Producto libro;//se crea la variable libro de tipo de dato complejo producto
	int codigoLibro;
	char repite = 1;
	char respuesta[MAX];

	do {//estructura de control repetitiva hacer mientras
		system("cls");
		tituloPrincipal();
		gotoxy(7,23);printf("Como codigos predeterminados para la prueba del programa tenemos desde el 1 hasta el 8");
		gotoxy(35,5);printf("Eliminar un libro por codigo");
		gotoxy(40,6);printf("Codigo del libro: ");
		leecad(linea, MAX);
		sscanf(linea, "%d", &codigoLibro);

		if (existeProducto(codigoLibro, &libro)) {//si consigue un libro con el codigo ingresado muestra sus datos al usuario

            gotoxy(2,7);printf("Codigo del producto: %d", libro.codigo);
			gotoxy(2,8);printf("Titulo: %s", libro.titulo);
			gotoxy(2,9);printf("Autor: %s", libro.autor);
			gotoxy(2,10);printf("Editorial: %s", libro.editorial);
			gotoxy(2,11);printf("Fecha de publicacion: %s", libro.fecha);
			gotoxy(2,12);printf("Anyo: %d", libro.anyo);
			gotoxy(2,13);printf("Cantidad: %d\n", libro.cantidad);
			gotoxy(2,14);printf("Precio del libro: %.1f $", libro.precio);



			gotoxy(34,16);printf("Desea eliminar el libro? [SI/NO]: ");
			leecad(respuesta, MAX);
			if (strcmp(respuesta, "SI") == 0 || strcmp(respuesta, "si") == 0) {//pregunta al usuario si está realmente seguro de eliminar el libro
				if (eliminarProducto(codigoLibro)) {
					gotoxy(36,18);printf("Libro eliminado exitosamente");
				} else {
					gotoxy(34,16);printf("El libro no pudo ser eliminado");
                    gotoxy(35,18);printf("Favor de intentarlo nuevamente");
				}
			}

		} else {
			gotoxy(2,7);printf("El libro de codigo %d no existe.", codigoLibro);
		}

		gotoxy(30,20);printf("Desea continuar eliminando libros? [SI/NO]: ");
		leecad(respuesta, MAX);

		if (!(strcmp(respuesta, "SI") == 0 || strcmp(respuesta, "si") == 0)) {
			repite = 0;
		}

	} while (repite);
}

char existeProducto(int codigoLibro, Producto *libro) //funcion que permite verificar si hay algun libro con el codigo que ingrese el usuario
{
	FILE *archivo;
	char existe;

	/* Abre el archivo en modo lectura */
	archivo = fopen("productos.dat", "rb");

	if (archivo == NULL) { /* Si no se pudo abrir el archivo, el valor de archivo es NULL */
		existe = 0;

	} else {
		existe = 0;

		/* Se busca el producto cuyo código coincida con codigoLibro */
		fread(&(*libro), sizeof(*libro), 1, archivo);
		while (!feof(archivo)) {
			if ((*libro).codigo == codigoLibro) {
				existe = 1;
				break;
			}
			fread(&(*libro), sizeof(*libro), 1, archivo);
		}

		/* Cierra el archivo */
		fclose(archivo);
	}

	return existe;
}

char ingresarProducto(Producto libro) //carga al archivo la informacion que ingrese el usuario del libro
{
	FILE *archivo;
	char anade;

	/* Abre el archivo para agregar datos al final */
	archivo = fopen("productos.dat", "ab");	/* Añade datos al final. Si el archivo no existe, es creado */

	if (archivo == NULL) { /* Si no se pudo abrir el archivo, el valor de archivo es NULL */
		anade = 0;

	} else {
		fwrite(&libro, sizeof(libro), 1, archivo);
		anade = 1;

		/* Cierra el archivo */
		fclose(archivo);
	}

	return anade;
}

char eliminarProducto(int codigoLibro) //elimina del archivo la informacion que desee el usuario
{
	FILE *archivo;
	FILE *auxiliar;
	Producto libro;
	char elimina;

	/* Abre el archivo para leer */
	archivo = fopen("productos.dat", "r+b");	/* Modo lectura/escritura. Si el archivo no existe, es creado */

	if (archivo == NULL) { /* Si no se pudo abrir el archivo, el valor de archivo es NULL */
		elimina = 0;

	} else {
		elimina = 0;
		fread(&libro, sizeof(libro), 1, archivo);
		while (!feof(archivo)) {
			if (libro.codigo == codigoLibro) {
				fseek(archivo, ftell(archivo) - sizeof(libro), SEEK_SET);
				libro.codigo = VALOR_CENTINELA;
				fwrite(&libro, sizeof(libro), 1, archivo);
				elimina = 1;
				break;
			}
			fread(&libro, sizeof(libro), 1, archivo);
		}

		/* Cierra el archivo */
		fclose(archivo);
	}

	return elimina;
}

int leecad(char *cad, int n) //funcion para leer caracteres
{
	int i, c;
	c = getchar();
	if (c == EOF) {
		cad[0] = '\0';
		return 0;
	}

	if (c == '\n') {
		i = 0;
	} else {

		cad[0] = c;
		i = 1;
	}

	for (; i < n - 1 && (c = getchar()) != EOF && c != '\n'; i++) {
		cad[i] = c;
	}
	cad[i] = '\0';

	if (c != '\n' && c != EOF)
		while ((c = getchar()) != '\n' && c != EOF);

	return 1;
}

void tituloPrincipal()
{
	cuadro(0,0,99,24);
    cuadro(1,1,98,4);
	gotoxy(46,2);printf("LIBRE-TE");
	gotoxy(41,3);printf("Los mejores libros");
}

void menuModificar(){
    Producto libro;
	int codigoLibro;
	char repite = 1;
	char respuesta[MAX];

	do {
		system("cls");
		tituloPrincipal();
		gotoxy(7,23);printf("Como codigos predeterminados para la prueba del programa tenemos desde el 1 hasta el 8");
		gotoxy(35,5);printf("Modificar el libro por codigo");
		gotoxy(40,6);printf("Codigo del libro: ");
		leecad(linea, MAX);
		sscanf(linea, "%d", &codigoLibro);

		if (existeProducto(codigoLibro, &libro)) {

			gotoxy(2,7);printf("Codigo del producto: %d", libro.codigo);
			gotoxy(2,8);printf("Titulo: %s", libro.titulo);
			gotoxy(2,9);printf("Autor: %s", libro.autor);
			gotoxy(2,10);printf("Editorial: %s", libro.editorial);
			gotoxy(2,11);printf("Fecha de publicacion: %s", libro.fecha);
			gotoxy(2,12);printf("Anyo: %d", libro.anyo);
			gotoxy(2,13);printf("Cantidad: %d\n", libro.cantidad);
			gotoxy(2,14);printf("Precio del libro: %.1f $", libro.precio);

			gotoxy(30,16);printf("Desea modificar este libro? [SI/NO]: ");
            leecad(respuesta, MAX);

            if ((strcmp(respuesta, "SI") == 0 || strcmp(respuesta, "si") == 0)) {
                eliminarProducto(codigoLibro);
                libro.codigo = codigoLibro;

                gotoxy(2,7);printf("                                                         ");
                gotoxy(2,7);printf("Ingrese el nuevo Titulo: ");
                leecad(libro.titulo, MAX);

                gotoxy(2,8);printf("                                                         ");
                gotoxy(2,8);printf("Ingrese el nuevo Autor: ");
                leecad(libro.autor, MAX);

                gotoxy(2,9);printf("                                                         ");
                gotoxy(2,9);printf("Ingrese la Editorial: ");
                leecad(libro.editorial, MAX);

                gotoxy(2,10);printf("                                                         ");
                gotoxy(2,10);printf("Fecha de publicacion dd/mm/aa: ");
                leecad(libro.fecha, MAX);

                libro.anyo = 0;
                gotoxy(2,11);printf("                                                         ");
                gotoxy(2,11);printf("Ingrese el Anyo de publicacion: ");
                leecad(linea, MAX);
                sscanf(linea, "%d", &libro.anyo);
                while(libro.anyo < 1||libro.anyo > 10000){
                    gotoxy(2,11);printf("                                                         ");
                    gotoxy(2,11);printf("Ingrese un anyo valido: ");
                    leecad(linea, MAX);
                    sscanf(linea, "%d", &libro.anyo);
                }

                libro.cantidad = 0;
                gotoxy(2,12);printf("                                                         ");
                gotoxy(2,12);printf("Cantidad de libros a ingresar: ");
                leecad(linea, MAX);
                sscanf(linea, "%d", &libro.cantidad);
                while(libro.cantidad < 1|| libro.cantidad > 1000000){
                    gotoxy(2,12);printf("                                                         ");
                    gotoxy(2,12);printf("Ingrese una cantidad valida: ");
                    leecad(linea, MAX);
                    sscanf(linea, "%d", &libro.cantidad);
                }

                libro.precio = 0.0;
                gotoxy(2,13);printf("                                                         ");
                gotoxy(2,13);printf("Precio del libro($): ");
                leecad(linea, MAX);
                sscanf(linea, "%f", &libro.precio);

                while(libro.precio < 1){
                    gotoxy(2,13);printf("                                                         ");
                    gotoxy(2,13);printf("Ingrese un precio valido($): ");
                    leecad(linea, MAX);
                    sscanf(linea, "%f", &libro.precio);
                }


                /* Se inserta el producto en el archivo */
                ingresarProducto(libro);
                gotoxy(30,16);printf("  El libro fue modificado correctamente  ");

         }
         }else {
            //El producto no existe
			gotoxy(2,7);printf("El libro de codigo %d no existe.", codigoLibro);
         }
        gotoxy(32,20);printf("Desea modificar otro libro? [SI/NO]: ");
        leecad(respuesta, MAX);
        if (!(strcmp(respuesta, "SI") == 0 || strcmp(respuesta, "si") == 0)) {
            repite = 0;
        }
		}while (repite);
}

void gotoxy(int x, int y){ //Funcion para darle estetica al programa, permite distribuir ordenadamente las impresiones de pantalla
	HANDLE hcon;
	hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y= y;
	SetConsoleCursorPosition(hcon,dwPos);
}

void cuadro(int xs, int ys, int xi, int yi){ //funcion que nos permite realizar ciertos recuadros para dar estetica
    for(int i = xs; i <= xi; i++){
        gotoxy(i, ys);printf("%c",196);
        gotoxy(i, yi);printf("%c",196);
    }
    for(int i = ys; i <= yi; i++){
        gotoxy(xs, i);printf("%c",179);
        gotoxy(xi, i);printf("%c",179);
    }

    gotoxy(xs, ys);printf("%c",218);
    gotoxy(xi, yi); printf("%c",217);
    gotoxy(xi,ys); printf("%c",191);
    gotoxy(xs, yi); printf("%c",192);
}

void desactivarMaximizar(){
	HWND consoleWindow;
	consoleWindow = GetConsoleWindow();
	SetWindowLong(consoleWindow, GWL_STYLE,GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
}

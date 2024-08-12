/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
 
 /*This function empties the stack until a left parenthesis is encountered.
 Operators removed from the stack are added to the postfix expression.*/
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
    char Current_Top;
    while (!Stack_IsEmpty(stack)) {
        Stack_Top(stack, &Current_Top);
        Stack_Pop(stack);
        if (Current_Top == '(')
            break;
        postfixExpression[(*postfixExpressionLength)++] = Current_Top;
    }
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
 
 /*If the stack is empty, or the operator at the top of the stack has lower precedence than 'c' or there 
 is a left bracket sign on the stack, the function pushes the current  operator 'c' onto the stack. In the
 other case, the function pops operators from the stack and adds them to the postfix expression.*/
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
	if (Stack_IsEmpty(stack)){
		Stack_Push(stack, c);
		return;
	}
	char Current_Top;
	Stack_Top(stack, &Current_Top);
	while (!(Current_Top == '(' || ((Current_Top == '+' || Current_Top == '-') && (c == '*' || c == '/')))){
		postfixExpression[(*postfixExpressionLength)++] = Current_Top;
		Stack_Pop(stack);
		if (Stack_IsEmpty(stack))
			break;
		Stack_Top(stack, &Current_Top);
	}
	Stack_Push(stack, c);
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
 
 /*The function processes infixExpression and converts it to postfixExpression. 
 It adds operands to the postfix expression or calls 
 doOperation function in case the current char is operator.*/
char *infix2postfix( const char *infixExpression ) {
	Stack stack;
    	Stack_Init(&stack);
    	unsigned postfixExpressionLength = 0;
    	char *postfixExpression = (char *)malloc(MAX_LEN * sizeof(char));
    	if (postfixExpression == NULL){
    		Stack_Dispose(&stack);
        	return NULL;
        }
    	for (int i = 0; infixExpression[i] != '\0'; i++) {
        	char c = infixExpression[i];

        	if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            		postfixExpression[postfixExpressionLength++] = c;
            	else if (c == '(')
			Stack_Push(&stack, c);
		else if (c == '+' || c == '-' || c == '*' || c == '/')
			doOperation(&stack, c, postfixExpression, &postfixExpressionLength);        		
        	else if (c == ')')
			untilLeftPar(&stack, postfixExpression, &postfixExpressionLength);
		else if (c == '='){
			untilLeftPar(&stack, postfixExpression, &postfixExpressionLength);
			postfixExpression[postfixExpressionLength++] = c;
			postfixExpression[postfixExpressionLength] = '\0';
			Stack_Dispose(&stack);
			return postfixExpression;
		}
	}
	Stack_Dispose(&stack);
	return NULL;
}


/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {
   
    char byte_1 = (value >> 24) & 0xFF;
    char byte_2 = (value >> 16) & 0xFF;
    char byte_3 = (value >> 8) & 0xFF;
    char byte_4 = value & 0xFF;

    Stack_Push(stack, byte_1);
    Stack_Push(stack, byte_2);
    Stack_Push(stack, byte_3);
    Stack_Push(stack, byte_4);
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop( Stack *stack, int *value ) {

    *value = 0;
    
    char byte_1, byte_2, byte_3, byte_4;
    Stack_Top(stack, &byte_4);
    Stack_Pop(stack);
    Stack_Top(stack, &byte_3);
    Stack_Pop(stack);
    Stack_Top(stack, &byte_2);
    Stack_Pop(stack);
    Stack_Top(stack, &byte_1);
    Stack_Pop(stack);
    
    
    *value |= ((int)byte_1 << 24) & 0xFF000000;
    *value |= ((int)byte_2 << 16) & 0x00FF0000;
    *value |= ((int)byte_3 << 8) & 0x0000FF00;
    *value |= (int)byte_4 & 0x000000FF;
	
}

/*My helper function to map a variable character to its corresponding constant value.*/
int var2constant(char var, VariableValue variableValues[], int variableValueCount){
 	for (int i=0; i<variableValueCount; i++){
 		if (var == variableValues[i].name)
 			return variableValues[i].value;
	}
	return 0;
}
 
/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */
 

/*This function first gets postfixExpression from the infix2postfix function and then calculates it. When an 
operand is encountered (variable or constant), the function pushes it onto the stack. When an operator is 
encountered, the function pops the required number of operands from the stack, performs the corresponding 
operation, and pushes the result back onto the stack*/
bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {
	Stack eval_stack;
    	Stack_Init(&eval_stack);
    	*value = 0;
    	char *postfixExpression = infix2postfix(infixExpression);
    	if (postfixExpression == NULL) {
    		Stack_Dispose(&eval_stack);
        	return false;
    	}
    	for (int i = 0; postfixExpression[i] != '\0'; i++) {
        	char c = postfixExpression[i];
		if (c == '+' || c == '-' || c== '/' || c == '*'){
			int operand_1=0, operand_2=0;
			if (!Stack_IsEmpty(&eval_stack)){	
				expr_value_pop(&eval_stack, &operand_1);
			}
			if (!Stack_IsEmpty(&eval_stack)){	
				expr_value_pop(&eval_stack, &operand_2);
			}
			int result;
			if (c == '/' && operand_1 == 0) {
                		free(postfixExpression);
                		Stack_Dispose(&eval_stack);
                		return false;
            		}
			switch(c){
				case '+':
					result = operand_1 + operand_2;
					break;
				case '-':
					result = operand_2 - operand_1;
					break;
				case '*':
					result = operand_1 * operand_2;
					break;
				case '/':
					result = operand_2 / operand_1;
					break;
			}
			expr_value_push(&eval_stack, result);
		}
		else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
			int constant = var2constant(c, variableValues, variableValueCount);
			expr_value_push(&eval_stack, constant);
		}
		else if (c >= '0' && c <= '9'){
			expr_value_push(&eval_stack, c -'0');
		}
	}
	if (!Stack_IsEmpty(&eval_stack)){	
		expr_value_pop(&eval_stack, value);
	}
    	free(postfixExpression);
    	Stack_Dispose(&eval_stack);
    	return true;
}

/* Konec c204.c */

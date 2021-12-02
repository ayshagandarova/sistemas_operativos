/*
nivelA.c - Adelaida Delgado (adaptación de nivel3.c)
Cada nivel incluye la funcionalidad de la anterior.
nivel A: Muestra el prompt, captura la línea de comandos, 
la trocea en tokens y chequea si se trata de comandos internos 
(cd, export, source, jobs, fg, bg o exit). Si son externos los ejecuta con execvp()
*/

#define _POSIX_C_SOURCE 200112L

#define DEBUGNA 0 // nivelA
#define DEBUGNB 1 // nivelB
#define DEBUGNC 1 // nivelc


#define PROMPT_PERSONAL 1 // si no vale 1 el prompt será solo el carácter de PROMPT

#define RESET_FORMATO "\033[0m"
#define NEGRO_T "\x1b[30m"
#define NEGRO_F "\x1b[40m"
#define GRIS "\x1b[94m"
#define ROJO_T "\x1b[31m"
#define VERDE_T "\x1b[32m"
#define AMARILLO_T "\x1b[33m"
#define AZUL_T "\x1b[34m"
#define MAGENTA_T "\x1b[35m"
#define CYAN_T "\x1b[36m"
#define BLANCO_T "\x1b[97m"
#define NEGRITA "\x1b[1m"

#define COMMAND_LINE_SIZE 1024
#define ARGS_SIZE 64
#define N_JOBS 24 // cantidad de trabajos permitidos

char const PROMPT = '$';
char mi_cmnd[COMMAND_LINE_SIZE];  // variable que contiene el último comando de nuestro shell


#include <errno.h>  //errno
#include <stdio.h>  //printf(), fflush(), fgets(), stdout, stdin, stderr, fprintf()
#include <stdlib.h> //setenv(), getenv()
#include <string.h> //strcmp(), strtok(), strerror()
#include <unistd.h> //NULL, getcwd(), chdir()
#include <sys/types.h> //pid_t
#include <sys/wait.h>  //wait()
#include <signal.h> //signal(), SIG_DFL, SIG_IGN, SIGINT, SIGCHLD
#include <fcntl.h> //O_WRONLY, O_CREAT, O_TRUNC
#include <sys/stat.h> //S_IRUSR, S_IWUSR
#include<stdbool.h>// Booleanos

int check_internal(char **args);
int internal_cd(char **args);
int internal_export(char **args);
int internal_source(char **args);
int internal_jobs(char **args);
int internal_bg(char **args);
int internal_fg(char **args);

char *read_line(char *line);
int parse_args(char **args, char *line);
int execute_line(char *line);

// nuevos:
void reaper(int signum);
void ctrlc(int signum);

static char mi_shell[COMMAND_LINE_SIZE]; //variable global para guardar el nombre del minishell

//static pid_t foreground_pid = 0;
struct info_process {
	pid_t pid;
	char status;
	char cmd[COMMAND_LINE_SIZE];
};
static struct info_process jobs_list[N_JOBS]; //Tabla de procesos. La posición 0 será para el foreground
int n_pids = 0;

void imprimir_prompt();

int check_internal(char **args) {
    if (!strcmp(args[0], "cd"))
        return internal_cd(args);
    if (!strcmp(args[0], "export"))
        return internal_export(args);
    if (!strcmp(args[0], "source"))
        return internal_source(args);
    if (!strcmp(args[0], "jobs"))
        return internal_jobs(args);
    if (!strcmp(args[0], "bg"))
        return internal_bg(args);
    if (!strcmp(args[0], "fg"))
        return internal_fg(args);
    if (!strcmp(args[0], "exit"))
        exit(0);
    return 0; // no es un comando interno
}

int internal_cd(char **args) {
    printf("[internal_cd()→ comando interno no implementado]\n");
    return 1;
} 

int internal_export(char **args) {
    printf("[internal_export()→ comando interno no implementado]\n");
    return 1;
}

int internal_source(char **args) {
    printf("[internal_source()→ comando interno no implementado]\n");
    return 1;
}

int internal_jobs(char **args) {
    #if DEBUGNA 
        printf("[internal_jobs()→ Esta función mostrará el PID de los procesos que no estén en foreground]\n");
    #endif
    for(int i = 1;i <= n_pids; i++){    //si hay algun comando en bg
        printf("[%d]\t%d\t%c\t%s\n",i,jobs_list[i].pid,
        jobs_list[i].status,jobs_list[i].cmd);
    }
    return 1;
}

int internal_fg(char **args) {
    #if DEBUGNA 
        printf("[internal_fg()→ Esta función enviará un trabajo detenido al foreground reactivando su ejecución, o uno del background al foreground ]\n");
    #endif
    return 1;
}

int internal_bg(char **args) {
    #if DEBUGNA 
        printf("[internal_bg()→ Esta función reactivará un proceso detenido para que siga ejecutándose pero en segundo plano]\n");
    #endif
    return 1;
}

void imprimir_prompt() {
#if PROMPT_PERSONAL == 1
    printf(NEGRITA ROJO_T "%s" BLANCO_T ":", getenv("USER"));
    printf(AZUL_T "MINISHELL" BLANCO_T "%c " RESET_FORMATO, PROMPT);
#else
    printf("%c ", PROMPT);

#endif
    fflush(stdout);  // stdout es la salida estándar 
    // el fflush -> fuerza a que el búfer de salida se imprima imprima 
    // el búfer de salida es 
    return;
}


char *read_line(char *line) {
  
    imprimir_prompt();
    char *ptr=fgets(line, COMMAND_LINE_SIZE, stdin); // leer linea
    if (ptr) {
        // ELiminamos el salto de línea (ASCII 10) sustituyéndolo por el \0
        char *pos = strchr(line, 10);
        if (pos != NULL){
            *pos = '\0';
        } 
	}  else {   //ptr==NULL por error o eof
        printf("\r");
        if (feof(stdin)) { //se ha pulsado Ctrl+D
            fprintf(stderr,"Bye bye\n");
            exit(0);
        }   
    }
    return ptr;
}

int parse_args(char **args, char *line) {
    int i = 0;

    args[i] = strtok(line, " \t\n\r");
    #if DEBUGNA 
        fprintf(stderr, GRIS "[parse_args()→ token %i: %s]\n" RESET_FORMATO, i, args[i]);
    #endif
    while (args[i] && args[i][0] != '#') { // args[i]!= NULL && *args[i]!='#'
        i++;
        args[i] = strtok(NULL, " \t\n\r");
        #if DEBUGNA 
            fprintf(stderr, GRIS "[parse_args()→ token %i: %s]\n" RESET_FORMATO, i, args[i]);
        #endif
    }
    if (args[i]) {
        args[i] = NULL; // por si el último token es el símbolo comentario
        #if DEBUGNA 
            fprintf(stderr, GRIS "[parse_args()→ token %i corregido: %s]\n" RESET_FORMATO, i, args[i]);
        #endif
    }
    return i;
}

int is_background(char **args){
    for (int i = 0; i < ARGS_SIZE && args[i] != NULL; i++){
        if (strcmp(args[i], "&") == 0){
            args[i] = NULL;
            return 0; // background
        }
    }
    return 1; //foreground
}


int jobs_list_add(pid_t pid, char status, char *cmd){
    if (n_pids != N_JOBS){
        n_pids++;
        jobs_list[n_pids].pid = pid;
        jobs_list[n_pids].status = status;
        strcpy(jobs_list[n_pids].cmd, cmd);
    }else {
        // hemos llegado al número máximo de trabajos
        printf("Numero de trabajos maximo alcanzado, libere alguno\n");
    }
    return EXIT_SUCCESS;
}


void jobs_list_remove(int pos){
    jobs_list[pos].pid = jobs_list[n_pids].pid;
    strcpy(jobs_list[pos].cmd,jobs_list[n_pids].cmd);
    jobs_list[pos].status = jobs_list[n_pids].status;
    n_pids--;
}

int jobs_list_find(pid_t pid){
    int i = 0;
    while (i <= n_pids){
        if (jobs_list[i].pid == pid){
            return i;
        }
        i++;
    }
    return -1;  // si no existe 
}


int execute_line(char *line) {
    char *args[ARGS_SIZE];
    pid_t pid;
    #if DEBUGNA
        int status;
    #endif
    char command_line[COMMAND_LINE_SIZE];
    //copiamos la línea de comandos sin '\n' para guardarlo en el array de structs de los procesos
    memset(command_line, '\0', sizeof(command_line)); 
    strcpy(command_line, line); //antes de llamar a parse_args() que modifica line

    memset(mi_cmnd, '\0', sizeof(command_line)); 
    strcpy(mi_cmnd, line); //antes de llamar a parse_args() que modifica line

    if (parse_args(args, line) > 0) {
        if (check_internal(args) == 0) { // si no es un comando interno
            // es un comando externo 
            int bkg = is_background(args);
            pid = fork();
            if (pid == 0){ // el hijo es el que ejecuta el comando externo
                signal(SIGCHLD, SIG_DFL);
                signal(SIGINT, SIG_IGN);  // ctrlc
                signal(SIGTSTP, SIG_IGN); // ctrlz
                
                //para los comandos del tipo rmdir "prueba dir"
                #if DEBUGNA || DEBUGNB
                    fprintf(stderr, GRIS "[execute_line()→ PID hijo: %i  (%s)]\n" RESET_FORMATO, getpid(), command_line);
                #endif 
                execvp(args[0],args);
                perror(args[0]);
                exit(EXIT_FAILURE);
            } else if(pid > 0){ // el padre espera a ser notificado de que el hijo ha acabado
            // el padre es el mini shell 
                #if DEBUGNA || DEBUGNB || DEBUGNC
                    fprintf(stderr, GRIS "[execute_line()→ PID padre: %i  (%s)]\n" RESET_FORMATO, getpid(), mi_shell);
                    fprintf(stderr, GRIS "[execute_line()→ PID hijo: %i  (%s)]\n" RESET_FORMATO, getpid(), command_line);
                #endif 
                if (bkg == 1){ // si es foreground
                    jobs_list[0].pid = pid;
                    jobs_list[0].status= 'E';
                    strcpy(jobs_list[0].cmd, mi_cmnd);

                    while(jobs_list[0].pid != 0){  // padre sólo se ejecutará mientras haya un proceso ejecutándose en foreground 
                        pause();    //esperamos a que acabe el hijo
                    }
                }else { // es background bkg == 0
                    jobs_list_add(pid,'E', command_line);
                    printf("[%d] %d\t%c\t%s \n", n_pids, pid,'E', mi_cmnd);
                }
                
                #if DEBUGNA 
                    fprintf(stderr, GRIS "[execute_line()→ Proceso hijo %i (%s) finalizado con exit(), estado: %i]\n" RESET_FORMATO, pid, command_line, status);
                #endif 
            }else if(pid < 0) {
                perror("Error fork():");
                exit(EXIT_FAILURE);
            }
            return 1;
        }
    }
    return 0;
}


void reaper(int signum){
    pid_t pidF; // pid finalizado
    int estado;
    signal(SIGCHLD, reaper);
    while ((pidF = waitpid(-1, &estado, WNOHANG))> 0) {
        if(jobs_list[0].pid == pidF){ //si es fg
            #if DEBUGNB   
            fprintf(stderr, GRIS "[reaper()→ Proceso hijo %i (%s) finalizado con exit code %i]\n" RESET_FORMATO, jobs_list[0].pid, jobs_list[0].cmd, estado);
            #endif
            jobs_list[0].pid = 0;
            jobs_list[0].status = 'F';
            memset(jobs_list[0].cmd,0, sizeof(jobs_list[0].cmd));
        }else {  // si es bkg
        
            int pos = jobs_list_find(pidF);
            if (pos != -1){
                if(WIFEXITED(estado)){
                    printf("\nTerminado PID %d (%s) en jobs_list[%d] con status %d\n",
                    jobs_list[pos].pid,jobs_list[pos].cmd,pos,WEXITSTATUS(estado));
                }else{
                    if(WIFSIGNALED(estado)){
                        printf("Terminado PID %d (%s) en jobs_list[%d] con status %d\n",
                        jobs_list[pos].pid,jobs_list[pos].cmd,pos,WTERMSIG(estado));
                    }
                }
                /*
                printf("\nTerminado PID %d (%s) en jobs_list[%d] con status %d\n",
                jobs_list[pos].pid,jobs_list[pos].cmd,pos,estado);*/
                fprintf(stderr, GRIS "[reaper()→ Proceso hijo %i en background (%s) finalizado con exit code %i]\n" RESET_FORMATO, jobs_list[pos].pid, jobs_list[pos].cmd, estado);
                jobs_list_remove(pos);
            }
        }
    }
}


void ctrlc(int signum){
    signal(SIGINT, ctrlc); 

    fprintf(stderr, GRIS "\n[ctrlc() -> soy el proceso con PID %i (%s) el proceso en foreground es %i (%s)]\n" RESET_FORMATO, getpid(), mi_shell, jobs_list[0].pid, jobs_list[0].cmd);
    if (jobs_list[0].pid>0){  // Si (hay un proceso en foreground) entonces //jobs_list[0].pid > 0
        if(strcmp(mi_shell, jobs_list[0].cmd) != 0){ //  Si (el proceso en foreground NO es el mini shell) entonces 
            kill(jobs_list[0].pid,SIGTERM);
        }else {
            fprintf(stderr, GRIS " [ctrlc()→ Señal %i enviada a %i (%s) por %i]\n" RESET_FORMATO, SIGTERM, jobs_list[0].pid, jobs_list[0].cmd, getpid());
        }        
    }else {
        fprintf(stderr, GRIS "[ctrlc() -> Señal %i no enviada por %i (%s) debido a que no hay proceso en foreground es %i (%s)]\n" RESET_FORMATO, SIGTERM, getpid(), mi_shell, jobs_list[0].pid, jobs_list[0].cmd);
    }
}

void ctrlz(int signum) {
    signal(SIGTSTP,ctrlz);
    fprintf(stderr, GRIS "\n[ctrlz() -> soy el proceso con PID %i (%s) el proceso en foreground es %i (%s)]\n" RESET_FORMATO, getpid(), mi_shell, jobs_list[0].pid, jobs_list[0].cmd);
    if (jobs_list[0].pid > 0){
        if (strcmp(mi_shell, jobs_list[0].cmd) != 0){
            kill(jobs_list[0].pid,SIGSTOP); 
            fprintf(stderr, GRIS "[ctrlz() -> Señal %i no enviada por %i (%s) debido a que no hay proceso en foreground es %i (%s)]\n" RESET_FORMATO, SIGTSTP, jobs_list[0].pid, jobs_list[0].cmd, getpid(), mi_shell);
            jobs_list[0].status = 'D';
            jobs_list_add(jobs_list[0].pid, jobs_list[0].status, jobs_list[0].cmd);
            jobs_list[0].pid = 0;
            jobs_list[0].status = 'F';
            strcpy(jobs_list[0].cmd,"");
            printf("[%d]\t%d\t%c\t%s\n",n_pids,jobs_list[n_pids].pid,
                jobs_list[n_pids].status,jobs_list[n_pids].cmd);
        }
    }
    
}


int main(int argc, char *argv[]) {
    char line[COMMAND_LINE_SIZE];
    memset(line, 0, COMMAND_LINE_SIZE);

    jobs_list[0].pid = 0;
    jobs_list[0].status = 'N';
    memset(jobs_list[0].cmd,0,sizeof(jobs_list[0].cmd));
    
    signal(SIGTSTP, ctrlz); // ctrlz
    signal(SIGINT, ctrlc);
    signal(SIGCHLD,reaper);
    

    while (1) {
        if(argc == 1){
            strcpy(mi_shell, argv[0]);
        }
        while (1) {
            if (read_line(line)) { // !=NULL
                execute_line(line);
            }
        }
    }
    return 0;
}







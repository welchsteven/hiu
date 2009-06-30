/*
 * main.c
 *
 *  Created on: Jun 29, 2009
 *      Author: Steven
 */

    #include <sys/types.h>
      #include <sys/stat.h>
      #include <fcntl.h>
      #include <termios.h>
      #include <stdio.h>

      #define BAUDRATE B38400
      #define PORT "/dev/ttyS1"

      main()
      {
        int fd,c, res;
        struct termios oldtio,newtio;
        char buf[255];

        /*
         * 	Open port for reading and writing and not as controlling tty
            because we don't want the program to end because of line noise sends CTRL-C.
         */


        fd = open(PORT, O_RDWR | O_NOCTTY );
        if (fd <0) {perror(PORT); exit(-1); }

        tcgetattr(fd,&oldtio); /* save current port settings */

        bzero(&newtio, sizeof(newtio));/* clear struct for new port settings */
		/*
		   BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
		   CRTSCTS : output hardware flow control (only used if the cable has
					 all necessary lines. See sect. 7 of Serial-HOWTO)
		   CS8     : 8n1 (8bit,no parity,1 stopbit)
		   CLOCAL  : local connection, no modem contol
		   CREAD   : enable receiving characters
		 */


        newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

        /*
          IGNPAR  : ignore bytes with parity errors
          ICRNL   : map CR to NL (otherwise a CR input on the other computer
                    will not terminate input)
          otherwise make device raw (no other input processing)
        */

        newtio.c_iflag = IGNPAR;

        /*
		 Raw output.
		*/

        newtio.c_oflag = 0;

        /* set input mode (non-canonical, no echo,...) */
        newtio.c_lflag = 0;

        newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
        newtio.c_cc[VMIN]     = 5;   /* blocking read until 5 chars received */

        /*
          now clean the modem line and activate the settings for the port
        */

        tcflush(fd, TCIFLUSH);
        tcsetattr(fd,TCSANOW,&newtio);


        while (STOP==FALSE) {       /* loop for input */
          res = read(fd,buf,255);   /* returns after 5 chars have been input */
          buf[res]=0;               /* so we can printf... */
          printf(":%s:%d\n", buf, res);
          if (buf[0]=='z') STOP=TRUE;
        }
        tcsetattr(fd,TCSANOW,&oldtio); /* Restore original serial port settings */
      }

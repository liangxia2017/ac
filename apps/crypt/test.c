/*

    XOR-Crypter
    (c) 2000-2002 Udo Wolter, uwp@dicke-aersche.de

    Reads a file/stdin/console-option and a keyfile/stdin/console-option
    and xors them. The result goes to the standard output/output-file.
    The key file should be equal or longer than the file which should be
    crypted (for obvious reasons: the longer the safer). Optional an
    offset can be given on which byte of the key the crypting begins.

    Usage: just execute it to see the options

    Should compile with:

    gcc -O2 crypt-XOR.c -o crypt-XOR

    on any Linux-System. Send me patches if it needs to be changed
    on other systems.

    BTW, this is truely the safest way of encryption (if the key is
    long enough), although it's symmetric encryption...

    V2.1 (2002-05-31): Add some more Options, minimal removing of
                       small non-beauties, name-change because
                       XOR-Crypt existed in freshmeat yet
    V2.0 (2002-05-29): Added some Options and changed behaviour (full length
                       messagesized key now is no must anymore as some of you
                       wished), major version update because of massive changes
    V1.6 (2001-02-01): Corrected version-dates & added version in help line
    V1.5 (2001-01-25): Even big files are supported (byte-for-byte)
    V1.4 (2000-11-21): Inserted GNU GPL hints
    V1.3 (2000-11-21): Add some documentation in the code
    V1.2 (2000-11-20): Initial Version

    Just mail me (uwp@dicke-aersche.de) if you find any errors, problems
    or maybe you have some patches I should include in this project. I think
    this project is not at the end yet, maybe even a graphical tool is possible
    and windows versions etc.

    REMINDER:

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License (GPL) as published
    by the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version. The GPL can be found here:

    http://www.gnu.org/copyleft/gpl.html

    or even here:

    http://www.dicke-aersche.de/gpl.txt

    This piece of software is free as long as you distribute it with the
    source.
   
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
   
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
    USA or take it from the web site as described earlier in this document
 
*/

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>

char vers[] = "2.0";
char pname[256];

void usage(char *progname, int full){
  if (full){
    fprintf(stderr,"%s: Version %s (c) 2000-2002 Udo Wolter UWP\n\n", progname, vers);
    fprintf(stderr,"Usage: %s [options]\n\n", progname);
    fprintf(stderr,"options:\n\n");  
    fprintf(stderr,"\t-version              show version (-v)\n");
    fprintf(stderr,"\t-help                 show this info (-h)\n");
    fprintf(stderr,"\t-msg       message    message to de/encrypt (-M)\n");
    fprintf(stderr,"\t-key       key        key for de/encrypting (-K)\n");
    fprintf(stderr,"\t-file      file       file to de/encrypt (-f)\n");
    fprintf(stderr,"\t-keyfile   keyfile    keyfile to use for de/encrypt (-k)\n");
    fprintf(stderr,"\t-offset    offset     offset for starting key, only valid\n");
    fprintf(stderr,"\t                      for a keyfile (-O)\n");
    fprintf(stderr,"\t-outfile   outfile    file for output, without this option\n");
    fprintf(stderr,"\t                      it just writes to stdout (-o)\n");
    fprintf(stderr,"\t-force                forces overwrite of existing files (-F)\n");
    fprintf(stderr,"\t-quiet                suppresses warnings, e.g. small key (-q)\n");
    fprintf(stderr,"\t-samename             if this is set, it writes to\n");
    fprintf(stderr,"\t                      file.xor (-s)\n\n");
    fprintf(stderr,"Hints:\n\n");
    fprintf(stderr,"File and key are a must, either as a pair of file/keyfile,\n");
    fprintf(stderr,"msg/key, file/key or msg/keyfile\n\n");
    fprintf(stderr,"Without force-option it won't overwrite existing files\n\n");
    fprintf(stderr,"-offset is optional, it's for moving the starting byte around\n");
    fprintf(stderr,"it cannot be used while using -key\n\n");
    fprintf(stderr,"if -outfile is not specified, it writes to stdout except\n");
    fprintf(stderr,"when -samename is set, here it just takes the required\n");
    fprintf(stderr,"argument of file and writes to file.xor\n\n");
    fprintf(stderr,"\042-\042 can be used for stdin but only either for file\n");
    fprintf(stderr,"or for keyfile\n\n");
    fprintf(stderr,"-file and -msg are exclusive as well as -key and -keyfile are\n");
    fprintf(stderr,"The same applies for -outfile and -samename\n\n");
  } else {
    fprintf(stderr,"For further assistance please use %s -help !\n", progname);
  }
  exit (-1);
}

/*
    Opens file & keyfile and sets filename.xor for possible smaename-flag
 */
FILE *in_file_open(char *in_file, long *in_size, char *in_out_xor){
  FILE *in_temp;
  struct stat in_filestatbuf;

  /*
      size = -1 to indicate stdin, no filename = no filename.xor
   */
  if (in_file[0] == '-'){
    *in_size = -1;
    in_out_xor = NULL;
    return(stdin);
  }
  in_temp = fopen(in_file,"r");
  if (!in_temp){
    perror(in_file);
    usage(pname, 0);
  }
  if (stat(in_file, &in_filestatbuf) == -1){
    perror(in_file);
    usage(pname, 0);
  }

  /*
     If in_out_xor != NULL we can fill it up, this is the
     decision if it's file or keyfile
  */
  if (in_out_xor){
    strcpy(in_out_xor,in_file);
    strcat(in_out_xor,".xor");
  }
  *in_size = (long)in_filestatbuf.st_size;
  return(in_temp);
}

/*
    if offset is bigger than the keyfile it has to be rewinded = starting at 0,
    simple rotation
 */
void test_rewind_keyfp(FILE *test_keyfp){
  if (feof(test_keyfp)){
    clearerr(test_keyfp);
    fseek(test_keyfp, 0, SEEK_SET);
  }
}

/*
    If key is coming from console-option, this is the rewind-routine to
    move the array-counter back to 0
 */
long test_rewind_key(char *test2_key, long test2_keycount){
  if (test2_keycount >= strlen(test2_key))
    return(0);
  return(test2_keycount);
}

/*
    write out one coded byte (there is no encryption or decryption, because
    it's symmetric crypting)
 */
void out_code(FILE *out_filefp, FILE *out_keyfp, char out_msg, char out_key,
              FILE *out_outfilefp){
  char out_filebuf, out_keybuf, out_result;

  if (out_filefp){
    out_filebuf = (unsigned char)fgetc(out_filefp);

    /*
        Last character of a file is EOF, this is my way to get rid of it
     */
    if (feof(out_filefp)){
      return;
    }
  } else {
    out_filebuf = out_msg;
  }
  if (out_keyfp){
    out_keybuf = (unsigned char)fgetc(out_keyfp);

    /*
        The feof-story is more worse here because of
        the rotation of the offset for keys that are
        too small
     */
    if (feof(out_keyfp)){
      test_rewind_keyfp(out_keyfp);
      out_keybuf = (unsigned char)fgetc(out_keyfp);
    }
  } else {
    out_keybuf = out_key;
  }
  out_result = (unsigned char) ((int)out_filebuf ^ (int)out_keybuf);
  /*
  fwrite(&out_result, 1, 1, out_outfilefp);
  */
  fputc(out_result, out_outfilefp);
}

/*
    Main routine for deciding which options are used to code correctly
 */
void code_all(FILE *code_filefp, FILE *code_keyfp, char *code_msg, char *code_key,
              long code_msgsize, long code_keysize, long code_offset, FILE *code_outfilefp){

  long code_i;
  long code_keycount;

  /*
     if the offset is bigger than the keyfile the offset will be
     displaced
  */
  code_offset = code_offset % code_keysize;

  if (code_keyfp){

    /*
        setting the file position in the key file
     */
    fseek(code_keyfp, code_offset, SEEK_SET);

    /*
        case: file & keyfile
     */
    if (code_filefp){
      while (!feof(code_filefp)){
        test_rewind_keyfp(code_keyfp);
        out_code(code_filefp, code_keyfp, '\0', '\0', code_outfilefp); 
      }

    /*
        case: msg & keyfile
     */
    } else {
      for(code_i = 0; code_i < code_msgsize; code_i++){
        test_rewind_keyfp(code_keyfp);
        out_code(NULL, code_keyfp, code_msg[code_i], '\0', code_outfilefp);
      }
    }

    /*
        case: file & key
     */
  } else {
    if (code_filefp){
      code_keycount = -1;
      while (!feof(code_filefp)){
        code_keycount += 1;
        code_keycount = test_rewind_key(code_key, code_keycount);
        out_code(code_filefp, NULL, '\0', code_key[code_keycount], code_outfilefp);
      }

    /*
        case: msg & key
     */
    } else {
      code_keycount = -1;
      for(code_i = 0; code_i < code_msgsize; code_i++){
        code_keycount += 1;
        code_keycount = test_rewind_key(code_key, code_keycount);
fprintf(stderr,"msg=%s key=%s count=%ld i=%ld\n",code_msg, code_key, code_keycount, code_i);
        out_code(NULL, NULL, code_msg[code_i], code_key[code_keycount], code_outfilefp);
      }
    }
  }
}

int main(int ac, char **av){

  FILE *filefp, *keyfp, *outfilefp;
  char outfile_xor[1024];
  long msgsize, keysize;
  int c;
  int msg_set = 0, key_set = 0, file_set = 0, keyfile_set = 0;
  int offset_set = 0, outfile_set = 0, samename_set = 0;
  int force_set = 0, quiet_set = 0;
  char *msg = NULL, *key = NULL, *file = NULL, *keyfile = NULL;
  char *outfile = NULL;
  long offset;
  int option_index = 0;
  struct stat teststat;

  static struct option long_options[] =
  {
    {"version", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},
    {"msg", required_argument, NULL, 'M'},
    {"key", required_argument, NULL, 'K'},
    {"file", required_argument, NULL, 'f'},
    {"keyfile", required_argument, NULL, 'k'},
    {"offset", required_argument, NULL, 'O'},
    {"outfile", required_argument, NULL, 'o'},
    {"force", no_argument, NULL, 'F'},
    {"quiet", no_argument, NULL, 'q'},
    {"samename", no_argument, NULL, 's'},
    {0,0,0,0}
  };

  strcpy(pname,av[0]);

  if (ac == 1)
    usage(pname, 1);

  while ((c = getopt_long_only(ac, av, "vhM:K:f:k:O:o:fqs", long_options, &option_index))!= -1) {
    switch(c){
    case 'v':
      fprintf(stderr,"Version = %s\n",vers);
      exit(0);
      break;
    case 'h':
      usage(pname, 1);
      break;
    case 'M':
      if (optarg[0] == '-'){
        fprintf(stderr,"-msg: cannot read from stdin !\n");
        usage(pname, 0);
      }
      msg_set = 1;
      msg = optarg;
      break;
    case 'K':
      if (optarg[0] == '-'){
	fprintf(stderr,"-key: cannot read from stdin !\n");
        usage(pname, 0);
      }
      key_set = 1;
      key = optarg;
      break;
    case 'f':
      if ((optarg[0] == '-') && (optarg[1] != '\0')){
	fprintf(stderr,"-file: argument missing !\n");
        usage(pname, 0);
      }
      file_set = 1;
      file = optarg;
      break;
    case 'k':
      if ((optarg[0] == '-') && (optarg[1] != '\0')){
	fprintf(stderr,"-keyfile: argument missing !\n");
        usage(pname, 0);
      }
      keyfile_set = 1;
      keyfile = optarg;
      break;
    case 'O':
      if (optarg[0] == '-'){
	fprintf(stderr,"-offset: argument missing !\n");
        usage(pname, 0);
      }
      offset_set = 1;
      offset = atol(optarg);
      break;
    case 'o':
      if (optarg[0] == '-'){
	fprintf(stderr,"-outfile: argument missing !\n");
        usage(pname, 0);
      }
      outfile_set = 1;
      outfile = optarg;
      break;
    case 'F':
      force_set = 1;
      break;
    case 'q':
      quiet_set = 1;
      break;
    case 's':
      samename_set = 1;
      break;
    case '?':
    default:
      usage(pname,1);
    }
  }

  /*
      Problem catching starts here, what's allowed and what's not
   */
  if (outfile_set && samename_set){
    fprintf(stderr,"-outfile and -samename must not be used together !\n");
    usage(pname, 0);
  }
  if (samename_set && !file_set){
    fprintf(stderr,"You must give a filename with -file for using -same_name !\n");
    usage(pname, 0);
  }
  if (samename_set && file_set && (file[0] == '-')){
    fprintf(stderr,"Using -samename with stdin is a nono !\n");
    usage(pname, 0);
  }
  if (keyfile_set && key_set){
    fprintf(stderr,"Only one of both key-options can be used !\n");
    usage(pname, 0);
  }
  if (file_set && msg_set){
    fprintf(stderr,"Only one of both message-options (either -msg or -file) can be used !\n");
    usage(pname, 0);
  }
  if (!keyfile_set && !key_set){
    fprintf(stderr,"You must use at least one of -key or -keyfile !\n");
    usage(pname, 0);
  }
  if (!file_set && !msg_set){
    fprintf(stderr,"You must use at least one of -msg or -file !\n");
    usage(pname, 0);
  }
  if (offset_set && keyfile_set && (keyfile[0] == '-')){
    fprintf(stderr,"You must not use -offset and -keyfile stdin together !\n");
    usage(pname, 0);
  }
  if (keyfile_set && file_set && (keyfile[0] == '-') && (file[0] == '-')){
    fprintf(stderr,"Only one of both (-file or -keyfile) can use with stdin at a time !\n");
    usage(pname, 0);
  }
  if ((key_set && (strlen(key) > 256)) || (msg_set && (strlen(msg) > 256))){
    fprintf(stderr,"option-limit: only 256 Bytes for -key or -msg is allowed !\n");
    usage(pname, 0);
  }

  /*
      Problem-zone ended, here we go for options connecting to the real data
      We're starting with the file stuff
   */
  if (file_set){
    filefp = in_file_open(file, &msgsize, outfile_xor);
  } else {

    /*
        size 0 = indicator for console-option
     */
    filefp = NULL;
    msgsize = 0;
  }

  if (keyfile_set){
    keyfp = in_file_open(keyfile, &keysize, NULL);
  } else {
    keyfp = NULL;
    keysize = 0;
  }

  if (!outfile_set){
    if (samename_set){
      outfile = outfile_xor;
    } else {
      outfilefp = stdout;
    }
  }

  /*
      Test, if the file exists. If it exists, it won't be
      overwritten
   */
  if (!force_set && !stat(outfile, &teststat)){
    fprintf(stderr,"%s already exists, not overwritten !\n", outfile);
    exit(0);
  }

  /*
      The real open of the output-file
   */
  if (outfilefp != stdout){
    outfilefp = fopen(outfile,"w");
    if (!outfilefp){
      perror(outfile);
      usage(pname, 0);
    }
  }

  /*
      Sizes can also be set if no files are used
   */
  if (msg_set){
    msgsize = strlen(msg);
  }
  if (key_set){
    keysize = strlen(key);
  }
 
  /*
      Warning: safe encryption shouldn't have a smaller key than the message-size
   */
  if (!quiet_set && (keysize != -1) && (msgsize != -1) && (keysize < msgsize)){
    fprintf(stderr,"Warning ! Key should have equal or bigger size than the messagesize !\n");
    fprintf(stderr,"A smaller keysize is risky and may have a negative impact on your data-safety !\n");
  }

  /*
      No offset -> offset starts at beginning of file
   */
  if (!offset_set)
    offset = 0;

  code_all(filefp, keyfp, msg, key, msgsize, keysize, offset, outfilefp);

  /*
      Closing the files
   */
  if (filefp && (filefp != stdin)){
    fclose(filefp);
  }
  if (keyfp && (keyfp != stdin)){
    fclose(keyfp);
  }
  if (outfilefp && (outfilefp != stdout)){
    fclose(outfilefp);
  }
  return(0);
}


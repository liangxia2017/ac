#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include    <time.h>
#include <pthread.h>

#include "dbinterface.h"



int PrepareSql(sqlite3 *db, sqlite3_stmt  **hs, char *SqlStr)
   {
       int        n, rc;
       n = 0;
       do
       {
           rc = sqlite3_prepare_v2(db, SqlStr, -1, hs, 0);
           if( (rc == SQLITE_BUSY) || (rc == SQLITE_LOCKED) )
           {
               n++;
               sleep(SQLTM_TIME);
           }
       }while( (n < SQLTM_COUNT) && ((rc == SQLITE_BUSY) || (rc == SQLITE_LOCKED)));
	   
       if( rc != SQLITE_OK)
       {
            return(0);
       }

       return(1);
  }

int StepSql(DB_CONTENT_T *pdb_content,sqlite3_stmt  *hs)
   {
       int        rc, n;

       n = 0;
       do
       {
           rc = sqlite3_step(hs);

         if( rc == SQLITE_LOCKED )
         {
            rc = sqlite3_reset(hs); /** Note: This will return SQLITE_LOCKED as well... **/
            n++;
            sleep(SQLTM_TIME);
         }
         else
           if( (rc == SQLITE_BUSY) )
           {
               sleep(SQLTM_TIME);

               n++;
           }
       }while( (n < SQLTM_COUNT) && ((rc == SQLITE_BUSY) || (rc == SQLITE_LOCKED)));

       if( n == SQLTM_COUNT )
       {
           pthread_mutex_lock(&pdb_content->m_lock);
           fprintf(stderr, "SqlStep Timeout  (rc = %d)\n",  rc);
           pthread_mutex_unlock(&pdb_content->m_lock);
       }

      if( n > 2 )
      {
         pthread_mutex_lock(&pdb_content->m_lock);
         fprintf(stderr, "SqlStep tries  %d\n",  n);
         pthread_mutex_unlock(&pdb_content->m_lock);
      }

       if( rc == SQLITE_MISUSE )
       {
           pthread_mutex_lock(&pdb_content->m_lock);
           fprintf(stderr, "sqlite3_step missuse \n");
          pthread_mutex_unlock(&pdb_content->m_lock);
       }

       return(rc);
   }

 int BeginTrans(DB_CONTENT_T *pdb_content)
   {
       int                rc;
       sqlite3_stmt    *bt_stmt;

       bt_stmt = NULL;

       if( !PrepareSql(pdb_content->rwDBConn, &bt_stmt, "BEGIN EXCLUSIVE TRANSACTION;") )
       {
           pthread_mutex_lock(&pdb_content->m_lock);
           fprintf(stderr, "Begin Transaction error  [%s %d] \n", __FUNCTION__,__LINE__);
          pthread_mutex_unlock(&pdb_content->m_lock);

           return(0);
       }

       rc = StepSql(pdb_content,bt_stmt);

       sqlite3_finalize(bt_stmt);

       if( rc != SQLITE_DONE )
       {
          pthread_mutex_lock(&pdb_content->m_lock);
          fprintf(stderr, "BeginTrans Timeout/Error [%s %d], Errorcode = %d \n", __FUNCTION__,__LINE__, rc);
          pthread_mutex_unlock(&pdb_content->m_lock);
           return(0);
       }

       return(1);
   }

   /** This ends the exclusive transaction...    **/
   int EndTrans(DB_CONTENT_T *pdb_content)
   {
       int         rc;
       sqlite3_stmt    *bt_stmt;


       if( !PrepareSql(pdb_content->rwDBConn, &bt_stmt, "COMMIT;") )
       {
           pthread_mutex_lock(&pdb_content->m_lock);
           fprintf(stderr, "EndTransaction prepare failed/timeout [%s %d]\n", __FUNCTION__,__LINE__);
           pthread_mutex_unlock(&pdb_content->m_lock);
           return(0);
       }

       rc = StepSql(pdb_content,bt_stmt);

       sqlite3_finalize(bt_stmt);

       if( rc != SQLITE_DONE )
       {
          pthread_mutex_lock(&pdb_content->m_lock);
           fprintf(stderr, "EndTrans Step Timeout [%s %d] (code = %d) \n",__FUNCTION__,__LINE__, rc);
          pthread_mutex_unlock(&pdb_content->m_lock);
           return(0);
       }

       return(1);
   }

   

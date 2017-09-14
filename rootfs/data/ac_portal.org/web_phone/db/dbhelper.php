<?php
class DAL {
    private static $__queries=0;
    private static $__PDO=null;
    private static function connect(){
        try {					
            self::$__PDO=new PDO("sqlite:/ac/data/ac_portal/web_phone/db/web_phone.s3db",null,null,array(PDO::ATTR_TIMEOUT=>100));
        } catch(PDOException $e) {
        	print_r($e);
            raiseError($e);
        }
    }
    /**
     *
     * @param <type> $sql
     * @param <type> $params
     * @return <type> $stmt
     *
     *
     * $stmt = $dbh->prepare("INSERT INTO REGISTRY (name, value) VALUES (?, ?)");
     * $stmt->bindParam(1, $name);
     * $stmt->bindParam(2, $value);
     * // insert one row
     * $name = 'one';
     * $value = 1;
     * $stmt->execute();
     *
     * $sql = "select * from SampleMarry where UserId=?";
     * $arr[0] = $userId;
     * return DAL::getall($sql,$arr);
     */
    private static function execute($sql,$params) {
        self::connect();
        try {
            $stmt=self::$__PDO->prepare($sql);            
            if($params!==null) {
                if(is_array($params) || is_object($params)) {
                    $i=1;
                    foreach($params as $param) {
                        $stmt->bindValue($i++,$param);
                        //$stmt->bindParam($i++,$param);
                    } 
                } else {
                    $stmt->bindValue(1,$params);
                    //$stmt->bindParam(1,$params);
                }
            }
            if($stmt->execute()) {
                self::$__queries++;
                return $stmt;
            } else {
                $err=$stmt->errorInfo();
                throw new PDOException($err[2],$err[1]);
            }
        } catch(PDOException $e) {
        print_r($e);
            //raiseError($e);
        }
    }
    /************************************************************************
    public function start
    ************************************************************************/
    //for insert start
    public static function insert($sql,$params=null) {
        $stmt=self::execute($sql,$params);
        return self::$__PDO->lastInsertId();
    }
    /**
     *
     * @param <type> $table
     * @param <type> $entity
     * @return <type>
        $data=new stdClass;
        $data->SampleBook_Id=$bookid;
        $data->UserId=$userId;
        $data->EducationType="School";
        $data->Name=$scoolname;
        $data->Subject="";
        $data->AttendedDate=strtotime($startdate);
        $data->FinishedDate=strtotime($enddate);
        return DAL::createRecord("SampleEducation",$item);
     */
    public static function createRecord($table,$entity) {
        $f=array();
        $v=array();
        $c=0;
        foreach($entity as $prop=>$val) {
            $f[]=$prop;
            $v[]=$val;
            $c++;
        }
        $sql="INSERT INTO $table (".implode(",",$f).") VALUES(".trim(str_repeat('?,',$c),",").")";
        return self::insert($sql,$v);
    }
    //for insert end
    //for delete start
    /**
     *
     * @param <type> $sql
     * @param <type> $params
     * @return <type>
        $sql="delete from SampleLived where id=?";
        $arr[] = $id;
        DAL::delete($sql,$arr);
     */
    public static function delete($sql,$params=null) {
        $stmt=self::execute($sql,$params);
        return $stmt->rowCount();
    }
    public static function remove($sql,$params=null) {
        $stmt=self::execute($sql,$params);
        return $stmt->rowCount();
    }
    //for delete end
    //for update start
    public static function update($sql,$params=null) {
        $stmt=self::execute($sql,$params);
        return $stmt->rowCount();
    }
    public static function updateRecord($table,$entity,$pkdfiledname) {
        $f=array();
        $v=array();
        foreach($entity as $prop=>$val) {
            if($prop==$pkdfiledname)continue;
            $f[]=$prop."=?";
            $v[]=$val;
        }
        $v[]=$entity->$pkdfiledname;
        $sql="UPDATE $table SET ".trim(implode(',',$f),',')." WHERE $pkdfiledname=?";
        return self::update($sql,$v);
    }
    //for update end
    //for query start
    public static function query($sql,$params=null) {
        $stmt=self::execute($sql,$params);
        return new DBRecordset($stmt);
    }
    //返回第一条记录的,第一列的值
    public static function getOne($sql,$params=null) {
        $stmt=self::execute($sql,$params);
        return $stmt->fetchColumn();
    }
    public static function getRow($sql,$params=null) {
        $stmt=self::execute($sql,$params);
        return $stmt->fetch(PDO::FETCH_OBJ);
    }
    public static function getall($sql,$params=null) {
        $stmt=self::execute($sql,$params);
        return $stmt->fetchAll(PDO::FETCH_ASSOC);
    }
    //for query end
    //for Transaction start
    public static function beginTransaction() {
        return self::$__PDO->beginTransaction();
    }
    public static function rollBack() {
        return self::$__PDO->rollBack();
    }
    public static function commit() {
        return self::$__PDO->commit();
    }
    //for Transaction end
    public static function pagination($countSql,$selectSql,$params,&$pageNow,$pageSize,&$count) {
        if($pageNow<=0)$pageNow=1;
        $count=self::getOne($countSql,$params);
        $pageCount=ceil($count/$pageSize);
        if($pageNow>$pageCount)$pageNow=$pageCount;
        if($pageNow<=0)$pageNow=1;
        $offset=($pageNow-1)*$pageSize;
        $sql=$selectSql.' LIMIT '.$offset.','.$pageSize;
        return self::query($sql,$params);
    }
    /************************************************************************
    public function end
    ************************************************************************/
}
class DBRecordset {
    private $PDOStatement;
    public function __construct(&$stmt) {
        $this->PDOStatement=&$stmt;
        $this->PDOStatement->setFetchMode(PDO::FETCH_OBJ);
    }
    public function next() {
        return $this->PDOStatement->fetch();
    }
    public function count() {
        //for mysql PDOStatement will return the number of rows in the resultset
        return $this->PDOStatement->rowCount();
    }
    public function getAllRows() {
        return $this->PDOStatement->fetchAll();
    }
    public function columnCount() {
        return $this->PDOStatement->columnCount();
    }
    public function free() {
        $this->PDOStatement=null;
    }
    public function __destruct() {
        $this->free();
    }
}
?>
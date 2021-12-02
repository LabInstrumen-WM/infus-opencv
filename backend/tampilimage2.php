<?php
 
 if($_SERVER['REQUEST_METHOD']=='GET'){
 $id = $_GET['id'];
 //$id =(string)$id;
 $sql = "select * from tbinfus where id = '$id'";
 require_once('koneksi.php');
 
 $r = mysqli_query($con,$sql);
 
 $result = mysqli_fetch_array($r);
 
 header('content-type: image/jpeg');
 
 echo $result['image'];
 
 mysqli_close($con);
 
 }else{
 echo "Error";
 }

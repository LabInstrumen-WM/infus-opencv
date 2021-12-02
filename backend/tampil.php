<?php
if ($_SERVER['REQUEST_METHOD'] == 'GET') {
  require_once('koneksi.php');

  $id = $_GET['id'];
  $sql = "SELECT * FROM tbinfus WHERE id=$id";
  $r = mysqli_query($con, $sql);

  $result = mysqli_fetch_array($r);
  mysqli_close($con);

  header('content-type: image/jpeg');

  //echo base64_decode($result['image']);
  echo $result['image'];
  //echo $result['id'];
  //echo'<img src="data:image/jpeg;base64 ,'.base64_encode($result['image']).'"/>'; 
}

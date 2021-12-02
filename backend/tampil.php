<?php
if($_SERVER['REQUEST_METHOD']=='GET')
{
$id=$_GET['id'];
$sql="SELECT * FROM tbinfus WHERE id=$id";
require_once('koneksi.php');
$r=mysqli_query($con,$sql);
$result = mysqli_fetch_array($r);
 
header('content-type: image/jpeg');
 
//echo base64_decode($result['image']);
echo $result['image'];
//echo $result['id'];
//echo'<img src="data:image/jpeg;base64 ,'.base64_encode($result['image']).'"/>'; 
mysqli_close($con);
}

?>
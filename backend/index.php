<!DOCTYPE html>
<html>

<head>
  <link rel="stylesheet" href="style.css" type="text/css" media="screen" title="no title" charset="utf-8">
</head>

<body>
  <h1>
    <center>Monitoring Cairan Infus</center>
  </h1>

  <?php
  $mysqli = mysqli_connect('localhost', 'jose', 'josekroos10', 'coba');
  if (!$mysqli)
    die("Can't connect to MySQL: " . mysqli_connect_error());
  
  $query = "SELECT * FROM tbinfus";
  $i = 0;
  if ($result = $mysqli->query($query)) {
    while ($row = $result->fetch_assoc()) {
      if ($i == 0) {
        echo '<div class="split left">';
      } else {
        echo '<div class="split right">';
      }

      echo '<div class="centered">';
      echo '<img src="data:image/jpeg;base64 ,' . base64_encode($row['image']) . '"/>';
      echo '</br>';
      echo 'ID=';
      echo $row['id'];
      echo '</br>';
      echo 'Volume=';
      echo $row['volume'];
      echo '</div>';
      echo '</div>';

      $i = $i + 1;
    }
  }
  ?>

</body>

</html>

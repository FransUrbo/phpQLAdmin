<?php
// shows configuration of phpQLAdmin
// config_detail.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();
require("./include/pql_config.inc");
global $config;

include("./header.html");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

$ldap = pql_get_subschemas($_pql->ldap_linkid);
?>
    <table cellspacing="0" cellpadding="3" border="0">
<?php
foreach($ldap as $x => $array) {
?>
      <th colspan="3" align="left"><?=$x?></th>
<?php
	if(!eregi($x, 'objectClasses')) {
?>
        <tr>
          <td>NAME</td>
          <td>OID</td>
          <td>SINGLE-VALUE</td>
          <td>DESC</td>
        </tr>

<?php    foreach($array as $value) { ?>
        <tr class="<?php table_bgcolor(); ?>">
<?php       echo "          <td>".$value['NAME']."</td>\n";
			echo "          <td>".$value['OID']."</td>\n";
			echo "          <td>".$value['SINGLE-VALUE']."</td>\n";
			echo "          <td>".$value['DESC']."</td>\n";
?>
        </tr>
<?php    } ?>
      </th>

<?php
	} else {
		$class = table_bgcolor(0);

		foreach($array as $value) {
?>
        <tr class="<?=$class?>">
          <td><?=$value['NAME']?></td>
		  <td><?=$value['OID']?></td>
          <td></td>
		  <td><?=$value['DESC']?></td>
        </tr>
<?php	$class = table_bgcolor(0);
        }
	}
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
    </table>
  </body>
</html>

<?php
// shows configuration of phpQLAdmin
// $Id: config_ldap.php,v 1.10 2004-02-14 14:01:00 turbo Exp $
//
session_start();
require("./include/pql_config.inc");

require("./left-head.html");
include("./header.html");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

$ldap = pql_get_subschemas($_pql->ldap_linkid);
if($type) {
	$tmp = $ldap; unset($ldap);

	$ldap[$type] = $tmp[$type];
	$attributetypes = $tmp['attributetypes'];
} else {
	$attributetypes = $ldap['attributetypes'];
}
$j = 0;
?>
    <table cellspacing="0" cellpadding="3" border="0">
<?php
foreach($ldap as $x => $array) {
?>
      <th colspan="3" align="left"><?=$x?>
<?php
	if(!eregi(lc($x), 'objectclasses')) {
?>

        <tr>
          <td><?=$LANG->_('Name')?></td>
          <td><?=$LANG->_('Oid')?></td>
          <td><?=$LANG->_('Single valued')?></td>
          <td><?=$LANG->_('Description')?></td>
        </tr>

<?php    foreach($array as $value) { ?>
        <tr class="<?php pql_format_table(); ?>">
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
		// Objectclasses
		$class = pql_format_table(0);
?>
        <br>
        <font size="1"><?=$LANG->_('Text in bold is a MUST, and non-bold is MAY')?>.</font>
        <br>
<?php	foreach($array as $value) { ?>
        <tr class="<?=$class?>">
          <td>
<?php		if($opera) { ?>
            <div id="el2Parent" class="parent" onclick="showhide(el<?=$j?>Spn, el<?=$j?>Img)">
              <img name="imEx" src="images/minus.png" border="0" alt="-" width="9" height="9" id="el<?=$j?>Img">
              <a class="item"><font color="black" class="heada"><?=$value['NAME']?></font></a>
            </div>
<?php		} else { ?>
            <div id="el<?=$j?>Parent" class="parent">
              <a class="item" onClick="if (capable) {expandBase('el<?=$j?>', true); expandBase('el<?=$j+1?>', true); return false;}">
                <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$j?>Img">
              </a>
        
              <a class="item"><font color="black" class="heada"><?=$value['NAME']?></font></a>
            </div>
<?php		} ?>

<?php		if($opera) { ?>
            <span id="el<?=$j?>Spn" style="display:''">
<?php		} else { ?>
            <div id="el<?=$j?>Child" class="child">
<?php		} ?>
<?php		for($i=0; $i < $value['MUST']['count']; $i++) { // MUST attributes ?>
              &nbsp;&nbsp;&nbsp;&nbsp;<b><?=$value['MUST'][$i]?></b><br>
<?php		} 

			for($i=0; $i < $value['MAY']['count']; $i++) {  // MAY attributes
?>
              &nbsp;&nbsp;&nbsp;&nbsp;<?=$value['MAY'][$i]?><br>
<?php		} ?>
<?php		if($opera) { ?>
            </span>
<?php		} else { ?>
            </div>
<?php		} ?>
          </td>
<?php		$j++; ?>
          <td>
<?php		if($opera) { ?>
            <div id="el<?=$j?>Parent" class="parent" onclick="showhide(el<?=$j?>Spn, el<?=$j?>Img)">
              <img name="imEx" src="images/minus.png" border="0" alt="-" width="9" height="9" id="el<?=$j?>Img">
              <a class="item"><font color="black" class="heada"><?=$value['OID']?></font></a>
            </div>

            <span id="el<?=$j?>Spn" style="display:''">
<?php		} else { ?>
            <div id="el<?=$j?>Parent" class="parent">
              <a class="item" onClick="if (capable) {expandBase('el<?=$j-1?>', true); expandBase('el<?=$j?>', true); return false;}">
                <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$j?>Img">
              </a>
              <a class="item"><font color="black" class="heada"><?=$value['OID']?></font></a>
            </div>

            <div id="el<?=$j?>Child" class="child">
<?php		} ?>
<?php		for($i=0; $i < $value['MUST']['count']; $i++) { // MUST attributes ?>
              &nbsp;&nbsp;&nbsp;&nbsp;<b><?php
			  	if($attributetypes[lc($value['MUST'][$i])]['OID']) {
					echo $attributetypes[lc($value['MUST'][$i])]['OID'];
				} else {
					// It's an alias
					foreach($attributetypes as $attrib) {
						if($attrib['ALIAS'][0])
						  if(lc($value['MUST'][$i]) == lc($attrib['ALIAS'][0]))
							echo $attrib['OID'];
					}
				} ?></b><br>
<?php       }

            for($i=0; $i < $value['MAY']['count']; $i++) { // MAY attributes ?>
              &nbsp;&nbsp;&nbsp;&nbsp;<?php
				if($attributetypes[lc($value['MAY'][$i])]['OID']) {
					echo $attributetypes[lc($value['MAY'][$i])]['OID'];
				} else {
					// It's an alias
					foreach($attributetypes as $attrib) {
						if($attrib['ALIAS'][0])
						  if(lc($value['MAY'][$i]) == lc($attrib['ALIAS'][0]))
							echo $attrib['OID'];
					}
				}?><br>

<?php       } ?>
<?php		if($opera) { ?>
            </span>
<?php		} else { ?>
            </div>
<?php		} ?>
          </td>
          <td></td>
          <td><?=$value['DESC']?></td>
        </tr>

<?php	$class = pql_format_table(0); $j++;
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
<?php require("./left-trailer.html"); ?>

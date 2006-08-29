<?php
// Automount information etc

// {{{ Retreive all hosts
if(pql_get_define("PQL_CONF_SUBTREE_COMPUTERS")) {
  $subrdn =  pql_get_define("PQL_CONF_SUBTREE_COMPUTERS") . ",";
}
$computerdn = $subrdn . $_GET["domain"];
$filter = "(&(objectClass=ipHost)(".pql_get_define("PQL_ATTR_CN")."=*))";
$computer_results = pql_search($_pql->ldap_linkid, $computerdn, $filter);

if(is_array($computer_results) and !@$computer_results[0]) {
  // Make sure it's a numbered array...
  $tmp = $computer_results;
  unset($computer_results);
  $computer_results[] = $tmp;
}
// }}}

// {{{ Retreive all automounts for all hosts
foreach($computer_results as $computer) {
  // Extract the FQDN for this object
  $computer = $computer[pql_get_define("PQL_ATTR_CN")];

  // Retreive autoMount maps for this host
  $automounts[$computer] = pql_get_automount_maps($_pql->ldap_linkid, $_GET["domain"], $computer);
  if(!is_array($automounts[$computer]))
    // No autoMount maps for this host, remove the entry from the array
    unset($automounts[$computer]);
}

if(file_exists($_SESSION["path"]."/.DEBUG_AUTOMOUNT")) {
  echo "<b>Automounts:</b>"; printr($automounts);
}
// }}}

// {{{ Table start and title
$titles = array('Device', 'Mountpoint', 'Filesystem', 'Mount option', 'Description');
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Automount information')."\n"?>
      <tr class="title">
<?php for($i=0; $titles[$i]; $i++) { ?>
        <td><?=$LANG->_($titles[$i])?></td>
<?php } ?>
      </tr>

<?php
$row = 'c1'; $host_cnt=1;
// }}}

// Go through the array - start with the first level - the FQDN
foreach($automounts as $host => $host_data) {
?>
      <tr class="<?=$row?>">
        <!-- 0: Host and Device -->
        <td>
<?php
  // {{{ Start of span
  // The '&nbsp;' included in the '$host' call is to make sure that the title isn't 'flopping' (changing width)
  if($_SESSION["opera"]) {
    pql_format_tree_opera($host_cnt, $host.'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;',
			  '', 0, '0Parent', array(0,
						  '0Spn', '0Img',
						  '1Spn', '1Img',
						  '2Spn', '2Img',
						  '3Spn', '3Img',
						  '4Spn', '4Img'));
  } else {
    pql_format_tree_other($host_cnt, $host.'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;',
			  '', 0, '0Parent', array(0,
						  '0Spn', '0Img',
						  '1Spn', '1Img',
						  '2Spn', '2Img',
						  '3Spn', '3Img',
						  '4Spn', '4Img'));
  }
// }}}

  // Go through the automount data
  $MOUNT = array();
  foreach($host_data as $mntpnt => $mnt_data) {
    if($mnt_data["automountinformation"]) {
      // {{{ Device mounted on mountpoint
      $tmp = preg_replace('/-fstype=/', '', $mnt_data["automountinformation"]);

      $opts = ereg_replace(' :.*', '', $tmp);
      $opts = split(',', $opts);
      
      $fs = $opts[0];
      
      $OPTS = '';
      for($j=1; $opts[$j]; $j++) {
	$OPTS .= $opts[$j];
	if($opts[$j+1])
	  $OPTS .= ',';
      }
      
      $MOUNT["Device"]		= ereg_replace('.*:', '', $tmp);
      $MOUNT["Mountpoint"][]	= $mntpnt;
      $MOUNT["Filesystem"][]	= $fs;
      $MOUNT["Mount option"][]	= $OPTS;
      $MOUNT["Description"][]	= $mnt_data["description"];
?>
            &nbsp;&nbsp;&nbsp;
            <img src="images/navarrow.png" width="9" height="9" border="0">
            &nbsp;<?=$MOUNT["Device"]."\n"?>
            <a class="tblItem" href="domain_edit_attributes.php?rootdn=<?=urlencode($_REQUEST["rootdn"])?>&domain=<?=urlencode($_REQUEST["domain"])?>&attrib=<?=pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")?>&host=<?=$host?>&mount=<?=urlencode($mnt_data["dn"])?>&action=delete&submit=2">
              <img src="images/del.png" align="right" width="12" height="12" border="0" alt="<?=pql_complete_constant($LANG->_("Delete %mount% from computer"), array('mount' => $mntpnt))?>">
            </a>

            <a class="tblItem" href="domain_edit_attributes.php?rootdn=<?=urlencode($_REQUEST["rootdn"])?>&domain=<?=urlencode($_REQUEST["domain"])?>&attrib=<?=pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")?>&host=<?=$host?>&mount=<?=urlencode($mnt_data["dn"])?>&action=modify&submit=2">
              <img src="images/edit.png" align="right" width="12" height="12" border="0" alt="<?=pql_complete_constant($LANG->_("Modify %mount%"), array('mount' => $mntpnt))?>">
            </a>

            <br>

<?php
// }}}
    } else {
      // {{{ Mountpoint with more than one device
      foreach($mnt_data as $mount => $mount_data) {
	if($mount_data["automountinformation"]) {
	  $tmp = preg_replace('/-fstype=/', '', $mount_data["automountinformation"]);

	  $opts = ereg_replace(' :.*', '', $tmp);
	  $opts = split(',', $opts);

	  $fs = $opts[0];

	  $OPTS = '';
	  for($j=1; $opts[$j]; $j++) {
	    $OPTS .= $opts[$j];
	    if($opts[$j+1])
	      $OPTS .= ',';
	  }

	  $MOUNT["Device"]		= ereg_replace('.*:', '', $tmp);
	  $MOUNT["Mountpoint"][]	= $mntpnt.'/'.$mount;
	  $MOUNT["Filesystem"][]	= $fs;
	  $MOUNT["Mount option"][]	= $OPTS;
	  $MOUNT["Description"][]	= $mount_data["description"];
?>
            &nbsp;&nbsp;&nbsp;
            <img src="images/navarrow.png" width="9" height="9" border="0">
            &nbsp;<?=$MOUNT["Device"]."\n"?>
            <a class="tblItem" href="domain_edit_attributes.php?rootdn=<?=urlencode($_REQUEST["rootdn"])?>&domain=<?=urlencode($_REQUEST["domain"])?>&attrib=<?=pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")?>&host=<?=$host?>&mount=<?=urlencode($mount_data["dn"])?>&action=delete&submit=2&view=<?=$_REQUEST["view"]?>">
              <img src="images/del.png" align="right" width="12" height="12" border="0" alt="<?=pql_complete_constant($LANG->_("Delete %mount% from computer"), array('mount' => $mntpnt.'/'.$mount))?>">
            </a>

            <a class="tblItem" href="domain_edit_attributes.php?rootdn=<?=urlencode($_REQUEST["rootdn"])?>&domain=<?=urlencode($_REQUEST["domain"])?>&attrib=<?=pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")?>&host=<?=$host?>&mount=<?=urlencode($mount_data["dn"])?>&action=modify&submit=2&view=<?=$_REQUEST["view"]?>">
              <img src="images/edit.png" align="right" width="12" height="12" border="0" alt="<?=pql_complete_constant($LANG->_("Modify %mount%"), array('mount' => $mntpnt))?>">
            </a>

            <br>

<?php 
      }
	//} else {
	// Mountpoint in a mountpoint...
	}
// }}}
    }
  }

  // {{{ End of span and td
  pql_format_tree_end();
 
?>
        </td>

<?php
// }}}

  // {{{ Go through the additional information (mountpoint, fs, options etc)
  for($j=1; $titles[$j]; $j++) {
?>
        <!-- <?=$j?>: <?=$titles[$j]?> -->
        <td>
<?php
    if($_SESSION["opera"]) {
      pql_format_tree_opera($host_cnt, '<br>', '', 0, $j.'Parent',
			    array($j,
				  '0Spn', '0Img',
				  '1Spn', '1Img',
				  '2Spn', '2Img',
				  '3Spn', '3Img',
				  '4Spn', '4Img'));
    } else {
      pql_format_tree_other($host_cnt, '<br>', '', 0, $j.'Parent',
			    array($j,
				  '0Spn', '0Img',
				  '1Spn', '1Img',
				  '2Spn', '2Img',
				  '3Spn', '3Img',
				  '4Spn', '4Img'));
    }

    $data = $MOUNT[$titles[$j]];
    foreach($data as $mount_point) {
      echo "           $mount_point<br>\n";
    }
?>
        </td>
<?php
    if($titles[$j+1])
      echo "\n";
  }
// }}}
?>
      </tr>

      <tr class="<?=$row?>">
        <td>
            &nbsp;&nbsp;&nbsp;
            <img src="images/navarrow.png" width="9" height="9" border="0">
            <a class="tblItem" href="domain_edit_attributes.php?rootdn=<?=urlencode($_REQUEST["rootdn"])?>&domain=<?=urlencode($_REQUEST["domain"])?>&attrib=<?=pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")?>&host=<?=$host?>&action=add&view=<?=$_REQUEST["view"]?>&submit=2">
	      &nbsp;<?=$LANG->_("Add new mount point")?>
            </a>

            <br>
        </td>

        <td></td>
        <td></td>
        <td></td>
        <td></td>
      </tr>
<?php

  if($row == 'c1')
    $row = 'c2';
  else
    $row = 'c1';

  $host_cnt++;
}


?>
    </th>
  </table>
<?php
/* Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>

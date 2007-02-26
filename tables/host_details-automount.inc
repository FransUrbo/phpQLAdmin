<?php
// Automount information etc
// $Id: host_details-automount.inc,v 1.6 2007-02-26 13:31:56 turbo Exp $

// {{{ Retreive automounts
if($_REQUEST["host"] == 'Global') {
  // Retreive ALL automounts for ALL hosts
  foreach($hosts as $host_dn) {
    // Retreive the FQDN for this host
    $host = $_pql->get_attribute($host_dn, pql_get_define("PQL_ATTR_CN"));
    
    // Retreive autoMount maps for this host
    $automounts[$host] = pql_get_automount_maps($host_dn);
    if(!is_array($automounts[$host]))
      // No autoMount maps for this host, remove the entry from the array
      unset($automounts[$host]);
  }
} else {
  // Retreive the FQDN for this host
  $host = $_pql->get_attribute($_REQUEST["host"], pql_get_define("PQL_ATTR_CN"));

  // Retreive automounts for specific host
  $automounts[$host] = pql_get_automount_maps($_REQUEST["host"]);
  if(!is_array($automounts[$host]))
    // No autoMount maps for this host, remove the entry from the array
    unset($automounts[$host]);
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
$row = 'c1'; $host_cnt=0;
// }}}

// Go through the array - start with the first level - the FQDN
foreach($automounts as $host => $host_data) {
?>
      <tr class="<?=$row?>">
        <!-- a: Host and Device -->
        <td>
<?php
  $letters = array('a', 'b', 'c', 'd', 'e');

  // {{{ Setup the highlighting when mouse hovers/leaves area
  $mouse = 'onmouseover="if (isDOM || isIE4) {';
  for($i=0; $letters[$i]; $i++)
    $mouse .= 'hilightBase(\'el'.$host_cnt.$letters[$i].'\', \'#CCFFCC\'); ';
  $mouse .= '}"';

  $mouse .= 'onmouseout="if (isDOM || isIE4) {';
  for($i=0; $letters[$i]; $i++)
    $mouse .= 'hilightBase(\'el'.$host_cnt.$letters[$i].'\', \'#D0DCE0\'); ';
  $mouse .= '}"';
  // }}}

  // {{{ Setup base exansion
  $expand1 = 'onclick="if (capable) {';
  for($i=0; $letters[$i]; $i++)
    $expand1 .= 'expandBase(\'el'.$host_cnt.$letters[$i].'\', true); ';
  $expand1 .= '}"';

  $expand2 = 'onclick="if (capable) {';
  for($i=0; $letters[$i]; $i++)
    $expand2 .= 'expandBase(\'el'.$host_cnt.$letters[$i].'\', false); ';
  $expand2 .= '}"';
  // }}}

  // {{{ Start of span
  // The '&nbsp;' included in the '$host' call is to make sure that the title isn't 'flopping' (changing width)
  if($_SESSION["opera"]) {
    pql_format_tree_opera($host_cnt, $host.'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;',
			  '', 0, '0aParent', array(0,
						   '0aChild', '0aImg',
						   '0bChild', '0bImg',
						   '0cChild', '0cImg',
						   '0dChild', '0dImg',
						   '0dChild', '0eImg'));
  } else {
    // Here 'a' is hard coded, because it's only called ONCE (for each host).
?>
          <div id="el<?=$host_cnt?>aParent" class="parent" <?=$mouse?>>
              <a class="item" <?=$expand1?>>
                <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$host_cnt?>aImg">
              </a>

              <a class="item" <?=$expand2?>>
                <span class="heada"><?=$host?></span>
              </a>
          </div>

          <div id="el<?=$host_cnt?>aChild" class="child" style="margin-bottom: 5px" <?=$mouse?>>
<?php
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
              <a class="tblItem" href="host_edit_attribute.php?host=<?=urlencode($host)?>&mount=<?=urlencode($mnt_data["dn"])?>&action=delete&attrib=<?=pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")?>&view=<?=$_REQUEST["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=pql_complete_constant($LANG->_("Delete %mount% from computer"), array('mount' => $mntpnt))?>"></a>
              <a class="tblItem" href="host_edit_attribute.php?host=<?=urlencode($host)?>&mount=<?=urlencode($mnt_data["dn"])?>&action=modify&attrib=<?=pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")?>&view=<?=$_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=pql_complete_constant($LANG->_("Modify %mount%"), array('mount' => $mntpnt))?>"></a>
              &nbsp;<?=$MOUNT["Device"]."\n"?>

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
              <a class="tblItem" href="host_edit_attribute.php?host=<?=urlencode($host)?>&mount=<?=urlencode($mount_data["dn"])?>&action=delete&attrib=<?=pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")?>&view=<?=$_REQUEST["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=pql_complete_constant($LANG->_("Delete %mount% from computer"), array('mount' => $mntpnt.'/'.$mount))?>"></a>
              <a class="tblItem" href="host_edit_attribute.php?host=<?=urlencode($host)?>&mount=<?=urlencode($mount_data["dn"])?>&action=modify&attrib=<?=pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")?>&view=<?=$_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=pql_complete_constant($LANG->_("Modify %mount%"), array('mount' => $mntpnt))?>"></a>
              &nbsp;<?=$MOUNT["Device"]."\n"?>

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
?>
          </div>
        </td>

<?php
// }}}

  // {{{ Go through the additional information (mountpoint, fs, options etc)
  for($j=1; $titles[$j]; $j++) {
?>
        <!-- <?=$letters[$j]?>: <?=$titles[$j]?> -->
        <td>
<?php
    if($_SESSION["opera"]) {
      pql_format_tree_opera($host_cnt, '<br>', '', 0, $j.'aParent',
			    array($j,
				  $j.'aChild', $j.'aImg',
				  $j.'bChild', $j.'bImg',
				  $j.'cChild', $j.'cImg',
				  $j.'dChild', $j.'dImg',
				  $j.'eChild', $j.'eImg'));
    } else {
?>
          <div id="el<?=$host_cnt?><?=$letters[$j]?>Parent" class="parent" <?=$mouse?>>
              <a class="item" <?=$expand1?>>
                <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$host_cnt?><?=$letters[$j]?>Img">
              </a>

              <a class="item" <?=$expand2?>>
                <span class="heada">&nbsp;</span>
              </a>
          </div>

          <div id="el<?=$host_cnt?><?=$letters[$j]?>Child" class="child" style="margin-bottom: 5px" <?=$mouse?>>
<?php
    }

    $data = $MOUNT[$titles[$j]];
    foreach($data as $mount_point) {
      // The '&nbsp;' and the image here is just so that all columns will be of the same
      // height! I'm only interested in 'height', hence 'width' in the img tag is only to
      // get it slightly of the left edge of it's frame.
      if(!$mount_point)
	$mount_point = "&nbsp;";
?>
            <img name="imEx" src="images/spacer_12x12.png" border="0" alt="+" width="4" height="12"><?=$mount_point?><br>
<?php
    }
?>
          </div>
        </td>
<?php
    if($titles[$j+1])
      echo "\n";
  }
// }}}
?>
      </tr>

      <tr class="<?=$row?>">
        <td colspan="5">
          <img src="images/navarrow.png" width="9" height="9" border="0">
          <a href="host_edit_attribute.php?host=<?=urlencode($host)?>&action=add&attrib=<?=pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")?>&view=<?=$_REQUEST["view"]?>">
	    &nbsp;<?=$LANG->_("Add new mount point")?>
          </a>
        </td>
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

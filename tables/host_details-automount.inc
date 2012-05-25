<?php
// Automount information etc
// $Id: host_details-automount.inc,v 1.8 2007-03-12 12:24:42 turbo Exp $

// {{{ table_go_through_details(host, mnt_pnt, mnt_data)
// This is here so we can have unlimited depth in mountpoints.
// It will call itself recursivly if it encounters sub-data...
function table_go_through_details($host, $mnt_pnt, $mnt_data) {
  global $MOUNT, $LANG;

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
    $MOUNT["Mountpoint"][]	= $mnt_pnt;
    $MOUNT["Filesystem"][]	= $fs;
    $MOUNT["Mount option"][]	= $OPTS;
    $MOUNT["Description"][]	= $mnt_data["description"];
?>
              &nbsp;&nbsp;&nbsp;
              <a class="tblItem" href="host_edit_attribute.php?host=<?php echo urlencode($host)?>&mount=<?php echo urlencode($mnt_data["dn"])?>&mountpoint=<?php echo $mnt_pnt?>&action=delete&attrib=<?php echo pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")?>&view=<?php echo $_REQUEST["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_("Delete %mount% from computer"), array('mount' => $mnt_pnt))?>"></a>
              <a class="tblItem" href="host_edit_attribute.php?host=<?php echo urlencode($host)?>&mount=<?php echo urlencode($mnt_data["dn"])?>&mountpoint=<?php echo $mnt_pnt?>&action=modify&attrib=<?php echo pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")?>&view=<?php echo $_REQUEST["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_("Modify %mount%"), array('mount' => $mnt_pnt))?>"></a>
              &nbsp;<?php echo $MOUNT["Device"]."\n"?>

            <br>

<?php
    // }}}

  } elseif(is_array($mnt_data)) {
    // {{{ A submount - call ourself recursivly to get the submounts information
    foreach($mnt_data as $sub_pnt => $sub_data) {
      $mount_point = $mnt_pnt;
      if(!preg_match('/\/$/', $mnt_pnt) and !preg_match('/^\//', $sub_pnt))
	$mount_point .= '/'.$sub_pnt;
      else
	$mount_point .= $sub_pnt;

      table_go_through_details($host, $mount_point, $sub_data);
    }
    // }}}
  }
}
// }}}

// {{{ Retreive automounts
if(($_REQUEST["host"] == 'Global') or !@$_REQUEST["host"]) {
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
  else {
    // Just because this is an array, doesn't mean that we actually have
    // mountpoints - we might have just added a basemount for a host
    // that didn't have any auto mounts...
    // Go through the array and see if we have any actual mount points...
    foreach($automounts[$host] as $mnt_pnt => $mnt_data) {
      if(!@$mnt_data) {
	// As suspected - just a base mount. Remove the entry from the array
	unset($automounts[$host]);
	break;
      }
    }
  }
}

if(file_exists($_SESSION["path"]."/.DEBUG_AUTOMOUNT")) {
  echo "<b>Automounts:</b>"; printr($automounts);
}
// }}}

// {{{ Table start and title
$titles = array('Device', 'Mountpoint', 'Filesystem', 'Mount option', 'Description');
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo $LANG->_('Automount information')."\n"?>
      <tr class="title">
<?php for($i=0; $titles[$i]; $i++) { ?>
        <td><?php echo $LANG->_($titles[$i])?></td>
<?php } ?>
      </tr>

<?php
$row = 'c1'; $host_cnt=0;
// }}}

// Go through the array - start with the first level - the FQDN
foreach($automounts as $host => $host_data) {
?>
      <tr class="<?php echo $row?>">
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
          <div id="el<?php echo $host_cnt?>aParent" class="parent" <?php echo $mouse?>>
              <a class="item" <?php echo $expand1?>>
                <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?php echo $host_cnt?>aImg">
              </a>

              <a class="item" <?php echo $expand2?>>
                <span class="heada"><?php echo $host?></span>
              </a>
          </div>

          <div id="el<?php echo $host_cnt?>aChild" class="child" style="margin-bottom: 5px" <?php echo $mouse?>>
<?php
  }
// }}}

  // {{{ Go through the automount data
  // - Output the 'Delete' and 'Modify' links and also the device
  // - Will call table_go_through_details() recursivly until all
  //   automount data have been retreived and displayed.
  $MOUNT = array();
  foreach($host_data as $mnt_pnt => $mnt_data)
    table_go_through_details($host, $mnt_pnt, $mnt_data);
// }}}

  // {{{ End of span and td
?>
          </div>
        </td>

<?php
// }}}

  // {{{ Go through the additional information (mountpoint, fs, options etc)
  for($j=1; $titles[$j]; $j++) {
?>
        <!-- <?php echo $letters[$j]?>: <?php echo $titles[$j]?> -->
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
          <div id="el<?php echo $host_cnt?><?php echo $letters[$j]?>Parent" class="parent" <?php echo $mouse?>>
              <a class="item" <?php echo $expand1?>>
                <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?php echo $host_cnt?><?php echo $letters[$j]?>Img">
              </a>

              <a class="item" <?php echo $expand2?>>
                <span class="heada">&nbsp;</span>
              </a>
          </div>

          <div id="el<?php echo $host_cnt?><?php echo $letters[$j]?>Child" class="child" style="margin-bottom: 5px" <?php echo $mouse?>>
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
            <img name="imEx" src="images/spacer_12x12.png" border="0" alt="+" width="4" height="12"><?php echo $mount_point?><br>
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
<?php

  if($row == 'c1')
    $row = 'c2';
  else
    $row = 'c1';

  $host_cnt++;
}

$host = '';
if($_REQUEST["host"] and ($_REQUEST["host"] != 'Global')) {
  $host = '&host='.$_REQUEST["host"];
}
?>
      <tr class="<?php echo $row?>">
        <td colspan="5">
          <img src="images/navarrow.png" width="9" height="9" border="0">
          <a href="host_edit_attribute.php?action=add<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_AUTOMOUNT_INFO")?>&view=<?php echo $_REQUEST["view"]?>">
	    &nbsp;<?php echo $LANG->_("Add new mount point")?>
          </a>
        </td>
      </tr>
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

<?php
// {{{ Table 'header'
?>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_("Host Control Access")."\n"?>
        <tr class="title">
          <td><?=$LANG->_("Hostname")?></td>
          <td><?=$LANG->_("IP Address")?></td>
        </tr>

<?php
// }}}

$row = 'c1'; $i=0;
foreach($hosts as $host_dn) {
  // {{{ Retreive values from this host
  $host   = $_pql->get_attribute($host_dn, pql_get_define("PQL_ATTR_CN"));
  $hostIP = $_pql->get_attribute($host_dn, pql_get_define("PQL_ATTR_IPHOSTNUMBER"));
// }}}

  // {{{ Retreive uniqueMember from this host
  $host_users = $_pql->get_attribute($host_dn, pql_get_define("PQL_ATTR_UNIQUE_GROUP"));
  if($host_users) {
	if(!is_array($host_users))
	  $host_users = array($host_users);
  } else
	$host_users = array();

  if(@$_REQUEST["domain"]) {
	// If called from the domain view->Host control

	// Go through the host users, verify that this user IS in this branch
	unset($tmp);
	foreach($host_users as $host_user) {
	  foreach($users as $user) {
		if(lc($host_user) == $user) {
		  // Save this
		  $tmp[] = $user;
		}
	  }
	}
	
	// Overwrite the list of users with the list of users located in this branch
	$host_users = $tmp;
  }
// }}}

  // {{{ Hostname(s)
?>	
        <tr class="<?=$row?>">
          <!-- HOSTNAME AND USERS -->
          <td>
<?php if($_SESSION["opera"]) { ?>
            <div id="el<?=$i?>aParent" class="parent" onclick="showhide(el<?=$i?>aSpn, el<?=$i?>aImg); showhide(el<?=$i?>bSpn, el<?=$i?>bImg);">
              <img name="imEx" src="images/minus.png" border="0" alt="-" width="9" height="9" id="el<?=$i?>aImg">
              <a class="item"><font color="black" class="heada"><?=$host?></font></a>
            </div>
<?php } else { ?>
            <div id="el<?=$i."aParent"?>" class="parent">
	      <a class="item" onClick="if (capable) {expandBase('el<?=$i?>a', true); return false;}">
                <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$i."aImg"?>">
              </a>
              <font color="black" class="heada"><?=$host?></font>
            </div>
<?php }
// }}}

  // {{{ User(s)
  if(@$host_users and !is_array($host_users))
	$host_users = array($host_users);
  if(@$host_users) {
		if($_SESSION["opera"]) {
?>
            <span id="el<?=$i?>aSpn" style="display:''">
<?php	} else { ?>
            <div id="el<?=$i."aChild"?>" class="child">
<?php	}

		for($j=0; $j < count($host_users); $j++) {
		  $user = $_pql->get_attribute($host_users[$j], pql_get_define("PQL_ATTR_UID"));
?>
             &nbsp;&nbsp;&nbsp;
             <img src="images/navarrow.png" width="9" height="9" border="0">&nbsp;<?=$user."\n"?>
               <a href="host_modify.php?host=<?=urlencode($host_dn)?>&userdn=<?=urlencode($host_users[$j])?>&action=remove_user_from_host">
                 <img src="images/del.png" width="12" height="12" border="0" alt="<?=pql_complete_constant($LANG->_("Delete %user% from computer"), array('user' => $user))?>">
               </a>
               <br>
<?php
		}
	  if($_SESSION["opera"]) {
?>
            </span>
<?php	  } else { ?>
            </div>
<?php	  }
  }
?>
          </td>
<?php
// }}}

  if($hostIP) {
	// {{{ IP Address
	$host_ip = '';
	if(is_array($hostIP)) {
	  for($z = 0; $z < count($hostIP); $z++) {
	    $host_ip .= $hostIP[$z];
	    if($hostIP[$z+1])
	      $host_ip .= ",&nbsp";
	  }
	} else
	  $host_ip = $hostIP;
?>

          <!-- IP ADDRESS -->
          <td>
<?php if($_SESSION["opera"]) { ?>
            <div id="el<?=$i?>bParent" class="parent" onclick="showhide(el<?=$i?>aSpn, el<?=$i?>aImg); showhide(el<?=$i?>bSpn, el<?=$i?>bImg);">
              <img name="imEx" src="images/minus.png" border="0" alt="-" width="9" height="9" id="el<?=$i?>bImg">
              <a class="item"><font color="black" class="heada"><?=$host_ip?></font></a>
            </div>

            <span id="el<?=$i?>bSpn" style="display:''">
<?php } else { ?>
            <div id="el<?=$i?>bParent" class="parent">
              <a class="item" onClick="if (capable) {expandBase('el<?=$i?>b', true); return false;}">
                <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$i?>bImg">
              </a>
              <a class="item"><font color="black" class="heada"><?=$host_ip?></font></a>
            </div>

            <div id="el<?=$i?>bChild" class="child">
<?php }
	echo "              <br>\n";

	if($_SESSION["opera"])
	  echo "            </span>\n";
	else
	  echo "            </div>\n";
	print "          </td>\n        </tr>\n";
	  
	if($row == 'c1')
	  $row = 'c2';
	else
	  $row = 'c1';
// }}}
  } else {
	// {{{ No IP address. Probably only a 'device' object
	if($_SESSION["opera"]) {
?>
          <!-- IP ADDRESS -->
          <td>
            <div id="el<?=$i?>bParent" class="parent" onclick="showhide(el<?=$i?>aSpn, el<?=$i?>aImg); showhide(el<?=$i?>bSpn, el<?=$i?>bImg);">
              <img name="imEx" src="images/minus.png" border="0" alt="-" width="9" height="9" id="el<?=$i?>bImg">
              <a class="item"><font color="black" class="heada"><?=$LANG->_('no ip')?></font></a>
            </div>

            <span id="el<?=$i?>bSpn" style="display:''">
<?php } else { ?>
            <div id="el<?=$i?>bParent" class="parent">
              <a class="item" onClick="if (capable) {expandBase('el<?=$i?>', true); return false;}">
                <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$i?>Img">
              </a>
              <a class="item"><font color="black" class="heada"><?=$LANG->_('no ip')?></font></a>
            </div>
          </td>
        </tr>
<?php }
// }}}
  }

  $i++;
} // end foreach($hosts)

// {{{ Table and file end
?>
        <tr class="c1">
          <td colspan="4">
            <img src="images/info.png" width="16" height="16" alt="" border="0">
<?php if(@$_REQUEST["domain"]) { ?>
            <?=$LANG->_('Note that you will only see users from \uthis\U branch here!')?>
<?php } else { ?>
            <?=$LANG->_('Note that you will have to go into a branch to \uadd\U users to a host.')?>
<?php } ?>
          </td>
        </tr>
      </th>
    </table>
<?php
require($_SESSION["path"]."/left-trailer.html");
// }}}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>

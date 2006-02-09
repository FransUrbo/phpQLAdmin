<?php
require($_SESSION["path"]."/include/pql_websrv.inc");

// Setup the object class filter
$filter = '(&(objectClass=ApacheSectionObj)(objectClass=device))';

// These must be a multileveled array for the pql_format_table_single()
// below to work!
$attribs = array(pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_DENY"),
		 pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ALLOW"));

// {{{ Retreive the Apache web server object(s)
$result = pql_search($_pql->ldap_linkid, 'ou=Web,'.$_REQUEST["domain"], $filter);
for($i=0; $i < count($result); $i++) {
  // Oki, we have webserver configuration(s) in LDAP.
  $name = $result[$i][pql_get_define("PQL_ATTR_CN")];

  foreach($attribs as $attrib) {
    if(@$result[$i][$attrib] && !@is_array($result[$i][$attrib])) {
      $tmp = $result[$i][$attrib];
      unset($result[$i][$attrib]);

      $result[$i][$attrib][] = $tmp;
    }
  }

  $web_srvs[$name] = $result[$i];
}
// }}}

if(@is_array($web_srvs)) {
  ksort($web_srvs);

  if(empty($_REQUEST["server"])) {
	// Use the first server as default (ie, if we're not called with a specific server to show)
	foreach($web_srvs as $_REQUEST["server"] => $data) break;
  } elseif(eregi('=', $_REQUEST["server"])) {
	// Server is a RDN				   -> server   = 'cn=testing.somwehere.tld,ou=WEB,dc=somewhere,dc=tld'
	// This will happen after changing a value with websrv_edit_attributes.php.
	$tmp = split('=', $_REQUEST["server"]);	// -> server[1]= 'testing.somwehere.tld,ou'
	$tmp = split(',', $web_srv[1]);			// -> server[0]= 'testing.somwehere.tld'
	$_REQUEST["server"] = $tmp[0];
  }
  $web_srv = $_REQUEST["server"];

  if(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
    echo "DEBUG: $web_srv";
    printr($web_srvs[$web_srv]);
  }
?>
  <table cellspacing="0" border="0" width="100%" cellpadding="0">
<?php foreach($web_srvs as $srv => $data) { ?>
    <a href="<?=$_SERVER["PHP_SELF"]."?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&view=".$_REQUEST["view"]."&server=$srv"?>"><img alt="/ <?=$srv?> \" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?=$srv?>"></a>
<?php } ?>
  </table>

  <br>
<?php
  $LINK_URL    = "websrv_edit_attributes.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"];
  $LINK_URL   .= "&server=".urlencode($web_srvs[$web_srv]["dn"])."&view=".$_REQUEST["view"];
  $img["mod"]  = '<img src="images/edit.png" width="12" height="12" alt="Modify value" border="0">';
  $img["del"]  = '<img src="images/del.png"  width="12" height="12" alt="Delete value" border="0">';
?>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Webserver administration')?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Server DN')?></td>
        <td><?=@$web_srvs[$web_srv]["dn"]?></td>
        <td></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Server location')?></td>
        <td><?=@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")]?></td>
        <td><a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")]?>"><?=$img["mod"]?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Server name')?></td>
        <td><?=@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")]?></td>
        <td><a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")]?>"><?=$img["mod"]?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Server administrator')?></td>
        <td><?=@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_ADMIN")]?></td>
        <td><a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_ADMIN")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_ADMIN")]?>"><?=$img["mod"]?></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Document root')?></td>
        <td><?=@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_DOCROOT")]?></td>
        <td><a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_DOCROOT")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_DOCROOT")]?>"><?=$img["mod"]?></a></td>
      </tr>

      <tr><td></td></tr>

<?php if(@is_array($web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")])) {
        $new_tr = 1;

        foreach($web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")] as $srv) {
           if($new_tr) { $new_tr = 0;
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Server alias')?></td>
<?php      } else { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
<?php      } ?>
        <td><?=$srv?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")?>&oldvalue=<?=$srv?>"><?=$img["mod"]?></a>
<?php if($srv) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")?>&oldvalue=<?=$srv?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>
<?php	} // END: foreach ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
        <td><a href="<?=$LINK_URL?>&action=add&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")?>"><?= pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server alias')))?></a></td>
        <td></td>
      </tr>
<?php } else {
	  // No Alias array
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Server alias')?></td>
        <td><a href="<?=$LINK_URL?>&action=add&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_ALIAS")?>"><?= pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server alias')))?></a></td>
        <td></td>
      </tr>
<?php } ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Script alias URL')?></td>
        <td><?=@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")]?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")]?>"><?=$img["mod"]?></a>
<?php if(@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")]) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_URL")]?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Script alias path')?></td>
        <td><?=@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")]?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")]?>"><?=$img["mod"]?></a>
<?php if(@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")]) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SCRIPT_PATH")]?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Error logfile')?></td>
        <td><?=@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")]?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")]?>"><?=$img["mod"]?></a>
<?php if(@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")]) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LOG_ERROR")]?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Transfer logfile')?></td>
        <td><?=@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")]?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")]?>"><?=$img["mod"]?></a>
<?php if(@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")]) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LOG_TRANSFER")]?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>

<?php if(@is_array($web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_OPTIONS")])) {
        $new_tr = 1;

        foreach($web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_OPTIONS")] as $srv) {
           if($new_tr) { $new_tr = 0;
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Additional option')?></td>
<?php      } else { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"></td>
<?php      } ?>
        <td><?=$srv?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_OPTIONS")?>&oldvalue=<?=$srv?>"><?=$img["mod"]?></a>
<?php if($srv) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_OPTIONS")?>&oldvalue=<?=$srv?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>
<?php	} // END: foreach ?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Additional option')?></td>
        <td><a href="<?=$LINK_URL?>&action=add&type="<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_OPTIONS")?>><?= pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server option')))?></a></td>
        <td></td>
      </tr>
<?php } else {
	  // No Alias array
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Additional option')?></td>
        <td><a href="<?=$LINK_URL?>&action=add&type="<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_OPTIONS")?>><?= pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('server option')))?></a></td>
        <td></td>
      </tr>
<?php } ?>

      <tr><td></td></tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Rewrite engine')?></td>
        <td><?=@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_ENGINE")]?></td>
        <td><a href="<?=$LINK_URL?>&toggle=1&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_ENGINE")?>&<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_ENGINE")?>=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_ENGINE")]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Toggle')?><?=pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_ENGINE")?>"></a>
</td>
      </tr>

<?php $class=pql_format_table(0);
      $url_tmp  = "$LINK_URL&attrib=".pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_RULE");

      if($web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_RULE")]) {
	$txt = array('tit' => $LANG->_('Rewrite rule'),
		     'add' => $LANG->_('Add rule'),
		     'mod' => $LANG->_('Modify rule'),
		     'del' => $LANG->_('Delete rule'));
		     $url = array('add' => $url_tmp."&action=add",
				  'del' => $url_tmp."&action=del",
				  'mod' => $url_tmp."&action=mod");
      } else {
	$txt = array('tit' => $LANG->_('Rewrite rule'),
		     'add' => $LANG->_('Add rule'));
	$url = array('add' => $url_tmp."&action=add");
      }
      pql_format_table_single($_pql->ldap_linkid, $txt, $url,
			      $web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_RULE")],
			      pql_get_define("PQL_ATTR_WEBSRV_SRV_REWRITE_RULE"), $class);
?>

      <tr><td></td></tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Section name')?></td>
        <td><?=@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_SECTION_NAME")]?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_SECTION_NAME")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_SECTION_NAME")]?>"><?=$img["mod"]?></a>
<?php if(@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_SECTION_NAME")]) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_SECTION_NAME")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_SECTION_NAME")]?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Set handler')?></td>
        <td><?=@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_SET_HANDLER")]?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_SET_HANDLER")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_SET_HANDLER")]?>"><?=$img["mod"]?></a>
<?php if(@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_SET_HANDLER")]) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_SET_HANDLER")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_SET_HANDLER")]?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Access Order')?></td>
        <td><?=@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ORDER")]?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ORDER")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ORDER")]?>"><?=$img["mod"]?></a>
<?php if(@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ORDER")]) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ORDER")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ORDER")]?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>

<?php $class=pql_format_table(0);
      $url_tmp  = "$LINK_URL&attrib=".pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_DENY");

      if($web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_DENY")]) {
	$txt = array('tit' => $LANG->_('Access (Deny)'),
		     'add' => $LANG->_('Deny access'),
		     'mod' => $LANG->_('Modify access'),
		     'del' => $LANG->_('Delete access'));
		     $url = array('add' => $url_tmp."&action=add",
				  'del' => $url_tmp."&action=del",
				  'mod' => $url_tmp."&action=mod");
      } else {
	$txt = array('tit' => $LANG->_('Access (Deny)'),
		     'add' => $LANG->_('Deny access'));
	$url = array('add' => $url_tmp."&action=add");
      }
      pql_format_table_single($_pql->ldap_linkid, $txt, $url,
			      $web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_DENY")],
			      pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_DENY"), $class);
?>

<?php $class=pql_format_table(0);
      $url_tmp  = "$LINK_URL&attrib=".pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ALLOW");

      if($web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ALLOW")]) {
	$txt = array('tit' => $LANG->_('Access (Allow)'),
		     'add' => $LANG->_('Allow access'),
		     'mod' => $LANG->_('Modify access'),
		     'del' => $LANG->_('Delete access'));
		     $url = array('add' => $url_tmp."&action=add",
				  'del' => $url_tmp."&action=del",
				  'mod' => $url_tmp."&action=mod");
      } else {
	$txt = array('tit' => $LANG->_('Access (Deny)'),
		     'add' => $LANG->_('Allow access'));
	$url = array('add' => $url_tmp."&action=add");
      }
      pql_format_table_single($_pql->ldap_linkid, $txt, $url,
			      $web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ALLOW")],
			      pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ALLOW"), $class);
?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Options')?></td>
        <td><?=@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")]?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")]?>"><?=$img["mod"]?></a>
<?php if(@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")]) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")]?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>

      <tr><td></td></tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('LDAP Server')?></td>
        <td><?=@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER")]?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER")]?>"><?=$img["mod"]?></a>
<?php if(@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER")]) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER")]?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('LDAP Login DN')?></td>
        <td><?=@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_DN")]?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_DN")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_DN")]?>"><?=$img["mod"]?></a>
<?php if(@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_DN")]) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_DN")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_DN")]?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('LDAP Login Password')?></td>
        <td><?=@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_PW")]?></td>
        <td>
          <a href="<?=$LINK_URL?>&action=mod&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_PW")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_PW")]?>"><?=$img["mod"]?></a>
<?php if(@$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_PW")]) { ?>
          <a href="<?=$LINK_URL?>&action=del&attrib=<?=pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_PW")?>&oldvalue=<?=$web_srvs[$web_srv][pql_get_define("PQL_ATTR_WEBSRV_LDAP_SERVER_LOGIN_PW")]?>"><?=$img["del"]?></a>
<?php } ?>
        </td>
      </tr>
    </th>
  </table>

<?php } else { ?>
  <br>

  <?=$LANG->_('No webserver configurations in this branch')?>.
<?php } ?>

  <br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=$LANG->_('Actions')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="websrv_add.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&view=<?=$_REQUEST["view"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('webserver configuration'))); ?></a></td>
      </tr>
    </th>
  </table>
<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>

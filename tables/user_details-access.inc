  <!-- Access list -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=$LANG->_('User access')?>
<?php
// {{{ Retreive values
$super_admin = 0;
foreach($_pql->ldap_basedn as $dn)  {
  $dn = pql_format_normalize_dn($dn);

  // {{{ Check the top-most (namingContexts) DN(s) if the user is super admin 
  if(pql_validate_administrator($dn, pql_get_define("PQL_ATTR_ADMINISTRATOR"), $_GET["user"])) {
	$super_admin = 1;
  }
// }}}

  if(!$super_admin) {
	// {{{ Find all branches the user is admin of
	$dom = $_pql->get_dn($dn, pql_get_define("PQL_ATTR_ADMINISTRATOR").'='.$_GET["user"]);
	if($dom) {
	  foreach($dom as $d) {
		// Filter out the RootDN
		if($d != $branch)
		  $domains_admins[] = $d;
	  }
	} else
	  $domains_admins = array();
// }}}
  
	// {{{ Find all branches the user is mailinglist admin of
	$dom = $_pql->get_dn($dn, pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM").'='.$_GET["user"]);
	if($dom) {
	  foreach($dom as $d)
		$domains_ezmlm[] = $d;
	} else
	  $domains_ezmlm = array();
// }}}

	// {{{ Find all branches the user is webserver admin of
	$dom = $_pql->get_dn($dn, pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV").'='.$_GET["user"]);
	if($dom) {
	  foreach($dom as $d)
		$domains_websrv[] = $d;
	} else
	  $domains_websrv = array();
// }}}

	// {{{ Find all branches the user is DNS admin of
	$dom = $_pql->get_dn($dn, pql_get_define("PQL_ATTR_ADMINISTRATOR_BIND9").'='.$_GET["user"]);
	if($dom) {
	  foreach($dom as $d)
		$domains_dns[] = $d;
	} else
	  $domains_dns = array();
// }}}
  }
}
// }}}

if(!$super_admin) {
  // {{{ Go through and show all the domains that the user is administrator of
  $class=pql_format_table(0);
  if(isset($domains_admins)) {
	asort($domains_admins);
	
	$class = pql_format_table(0);
	$url_tmp  = "domain_edit_attributes.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"];
	$url_tmp .= "&view=".$_REQUEST["view"]."&attrib=".pql_get_define("PQL_ATTR_ADMINISTRATOR");
	
	$tbl_txt = array('tit' => $LANG->_('Administrator in branch'),
					 'add' => $LANG->_('Grant user administrator access to domain'),
					 'del' => $LANG->_('Disallow user administrator access to domain'));
	$tbl_url = array('add' => $url_tmp."&user=".$url["user"]."&submit=3&action=add",
					 'del' => $url_tmp."&".pql_get_define("PQL_ATTR_ADMINISTRATOR")."=".$url["user"]."&submit=4&action=delete");
	pql_format_table_single($tbl_txt, $tbl_url, $domains_admins, 'branchdn', $class);
  }
// }}}

  // {{{ Go through and show all the domains that the user is MAILINGLIST administrator of
  $class=pql_format_table(0);
  if(isset($domains_ezmlm)) {
	asort($domains_ezmlm);
	
	$class = pql_format_table(0);
	$url_tmp  = "domain_edit_attributes.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&branchdn=".$url["domain"];
	$url_tmp .= "&view=".$_REQUEST["view"]."&attrib=".pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM");
	
	$tbl_txt = array('tit' => $LANG->_('Mailinglist administrator in branch'),
					 'add' => $LANG->_('Grant user mailinglist administrator access to domain'),
					 'del' => $LANG->_('Disallow user mailinglist administrator access to domain'));
	$tbl_url = array('add' => $url_tmp."&user=".$url["user"]."&submit=3&action=add",
					 'del' => $url_tmp."&".pql_get_define("PQL_ATTR_ADMINISTRATOR").'='.$url["user"]."&submit=4&action=delete");
	pql_format_table_single($tbl_txt, $tbl_url, $domains_ezmlm,
							pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM"), $class);
  }
// }}}

  // {{{ Go through and show all the domains that the user is WEBSERVER administrator of
  $class=pql_format_table(0);
  if(isset($domains_websrv)) {
	asort($domains_websrv);
	
	$class = pql_format_table(0);
	$url_tmp  = "domain_edit_attributes.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&branchdn=".$url["domain"];
	$url_tmp .= "&view=".$_REQUEST["view"]."&attrib=".pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV");
	
	$tbl_txt = array('tit' => $LANG->_('Webserver administrator in branch'),
					 'add' => $LANG->_('Grant user webserver administrator access to domain'),
					 'del' => $LANG->_('Disallow user webserver administrator access to domain'));
	$tbl_url = array('add' => $url_tmp."&user=".$url["user"]."&submit=3&action=add",
					 'del' => $url_tmp."&".pql_get_define("PQL_ATTR_ADMINISTRATOR").'='.$url["user"]."&submit=4&action=delete");
	pql_format_table_single($tbl_txt, $tbl_url, $domains_websrv,
							pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV"), $class);
  }
// }}}

  // {{{ Go through and show all the domains that the user is DNS administrator of
  $class=pql_format_table(0);
  if(isset($domains_dns)) {
	asort($domains_dns);
	
	$class = pql_format_table(0);
	$url_tmp  = "domain_edit_attributes.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&branchdn=".$url["domain"];
	$url_tmp .= "&view=".$_REQUEST["view"]."&attrib=".pql_get_define("PQL_ATTR_ADMINISTRATOR_BIND9");
	
	$tbl_txt = array('tit' => $LANG->_('DNS administrator in branch'),
					 'add' => $LANG->_('Grant user DNS administrator access to domain'),
					 'del' => $LANG->_('Disallow user DNS administrator access to domain'));
	$tbl_url = array('add' => $url_tmp."&user=".$url["user"]."&submit=3&action=add",
					 'del' => $url_tmp."&".pql_get_define("PQL_ATTR_ADMINISTRATOR").'='.$url["user"]."&submit=4&action=delete");
	pql_format_table_single($tbl_txt, $tbl_url, $domains_dns,
							pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV_BIND9"), $class);
  }
// }}}
}

// {{{ Are the user allowed to create branches (i.e. is the user listed in the Root DN)?
if($_SESSION["ALLOW_BRANCH_CREATE"]) {
  $class=pql_format_table(0);
?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Super administrator')?></td>
<?php	if($super_admin) { ?>
        <td><?=$LANG->_('Yes')?></td>
        <td>
          <a href="domain_edit_attributes.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&branchdn=superadmin&attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR")?>&<?=pql_get_define("PQL_ATTR_ADMINISTRATOR")?>=<?=$url["user"]?>&submit=4&action=delete&view=<?=$_GET["view"]?>">
            <img src="images/del.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Disallow user to create domains below %domain%'), array('domain' => $_GET["rootdn"])); ?>">
          </a>
        </td>
<?php } else { ?>
        <td><?=$LANG->_('No')?></td>
        <td>
          <a href="domain_edit_attributes.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&branchdn=superadmin&attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR")?>&<?=pql_get_define("PQL_ATTR_ADMINISTRATOR")?>=<?=$url["user"]?>&submit=4&action=add&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Allow user to create domains below %domain%'), array('domain' => $_GET["domain"])); ?>">
          </a>
        </td>
<?php   } ?>
      </tr>

<?php
}
// }}}

// {{{ Are the user allowed to administrate mailserver information?
?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Administrate global mailservers')?></td>
<?php if($controlsadministrator) { ?>
        <td><?=$LANG->_('Yes')?></td>
        <td>
<?php	if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
          <a href="domain_edit_attributes.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&branchdn=superadmin&attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS")?>&<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS")?>=<?=$url["user"]?>&submit=4&action=delete&view=<?=$_GET["view"]?>">
            <img src="images/del.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Disallow user to manage global mailservers'), array('domain' => $_GET["rootdn"])); ?>">
          </a>
<?php	} ?>
        </td>
<?php } else {
		if($super_admin) {
?>
        <td><?=$LANG->_('Yes (user is superadmin)')?></td>
<?php	} else {
?>
        <td><?=$LANG->_('No')?></td>
<?php	} ?>
        <td>
<?php	if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
          <a href="domain_edit_attributes.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&branchdn=superadmin&attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS")?>&<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS")?>=<?=$url["user"]?>&submit=4&action=add&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Allow user to create domains below %domain%'), array('domain' => $_GET["domain"])); ?>">
          </a>
<?php	} ?>
        </td>
<?php } ?>
      </tr>

<?php
// }}}

// {{{ Are the user allowed to administrate webserver information?
?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Administrate global webservers')?></td>
<?php if($webserveradministrator) { ?>
        <td><?=$LANG->_('Yes')?></td>
        <td>
<?php	if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
          <a href="domain_edit_attributes.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&branchdn=superadmin&attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV")?>&<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV")?>=<?=$url["user"]?>&submit=4&action=delete&view=<?=$_GET["view"]?>">
            <img src="images/del.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Disallow user to manage global webservers'), array('domain' => $_GET["rootdn"])); ?>">
          </a>
<?php	} ?>
        </td>
<?php } else {
		if($super_admin) {
?>
        <td><?=$LANG->_('Yes (user is superadmin)')?></td>
<?php	} else {
?>
        <td><?=$LANG->_('No')?></td>
<?php	} ?>
        <td>
<?php	if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>
          <a href="domain_edit_attributes.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&branchdn=superadmin&attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV")?>&<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV")?>=<?=$url["user"]?>&submit=4&action=add&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Allow user to manage global webservers'), array('domain' => $_GET["domain"])); ?>">
          </a>
<?php	} ?>
        </td>
<?php } ?>
      </tr>

<?php
// }}}
?>
    </th>
  </table>

  <br><br>
<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>

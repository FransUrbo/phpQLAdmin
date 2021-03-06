    <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Webserver administration: \b%server%\B'), array('server' => $server_reference))?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Directory DN')?></td>
        <td><?php echo @$virt_host_dirs[$virt_host_dir]["dn"]?></td>
        <td></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Set handler')?></td>
        <td><?php echo @$virt_host_dirs[$virt_host_dir][pql_get_define("PQL_ATTR_WEBSRV_SRV_SET_HANDLER")]?></td>
        <td>
          <a href="<?php echo $LINK_URL?>&action=mod&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SRV_SET_HANDLER")?>&oldvalue=<?php echo $virt_host_dirs[$virt_host_dir][pql_get_define("PQL_ATTR_WEBSRV_SRV_SET_HANDLER")]?>"><?php echo $img["mod"]?></a>
<?php if(@$virt_host_dirs[$virt_host_dir][pql_get_define("PQL_ATTR_WEBSRV_SRV_SET_HANDLER")]) { ?>
          <a href="<?php echo $LINK_URL?>&action=del&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SRV_SET_HANDLER")?>&oldvalue=<?php echo $virt_host_dirs[$virt_host_dir][pql_get_define("PQL_ATTR_WEBSRV_SRV_SET_HANDLER")]?>"><?php echo $img["del"]?></a>
<?php } ?>
        </td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Access Order')?></td>
        <td><?php echo @$virt_host_dirs[$virt_host_dir][pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ORDER")]?></td>
        <td>
          <a href="<?php echo $LINK_URL?>&action=mod&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ORDER")?>&oldvalue=<?php echo $virt_host_dirs[$virt_host_dir][pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ORDER")]?>"><?php echo $img["mod"]?></a>
<?php if(@$virt_host_dirs[$virt_host_dir][pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ORDER")]) { ?>
          <a href="<?php echo $LINK_URL?>&action=del&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ORDER")?>&oldvalue=<?php echo $virt_host_dirs[$virt_host_dir][pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ORDER")]?>"><?php echo $img["del"]?></a>
<?php } ?>
        </td>
      </tr>

<?php $class=pql_format_table(0);
      $url_tmp  = "$LINK_URL&attrib=".pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_DENY");

      if($virt_host_dirs[$virt_host_dir][pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_DENY")]) {
	$txt = array('tit' => $LANG->_('Access (Deny)'),
		     'add' => $LANG->_('Deny access'),
		     'mod' => $LANG->_('Modify access'),
		     'del' => $LANG->_('Delete access'));
	$link_url = array('add' => $url_tmp."&action=add",
			  'del' => $url_tmp."&action=del",
			  'mod' => $url_tmp."&action=mod");
      } else {
	$txt = array('tit' => $LANG->_('Access (Deny)'),
		     'add' => $LANG->_('Deny access'));
	$link_url = array('add' => $url_tmp."&action=add");
      }
      pql_format_table_single($txt, $link_url,
			      $virt_host_dirs[$virt_host_dir][pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_DENY")],
			      pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_DENY"), $class);
?>

<?php $class=pql_format_table(0);
      $url_tmp  = "$LINK_URL&attrib=".pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ALLOW");

      if($virt_host_dirs[$virt_host_dir][pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ALLOW")]) {
	$txt = array('tit' => $LANG->_('Access (Allow)'),
		     'add' => $LANG->_('Allow access'),
		     'mod' => $LANG->_('Modify access'),
		     'del' => $LANG->_('Delete access'));
	$link_url = array('add' => $url_tmp."&action=add",
			  'del' => $url_tmp."&action=del",
			  'mod' => $url_tmp."&action=mod");
      } else {
	$txt = array('tit' => $LANG->_('Access (Deny)'),
		     'add' => $LANG->_('Allow access'));
	$link_url = array('add' => $url_tmp."&action=add");
      }
      pql_format_table_single($txt, $link_url,
			      $virt_host_dirs[$virt_host_dir][pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ALLOW")],
			      pql_get_define("PQL_ATTR_WEBSRV_SRV_ACCESS_ALLOW"), $class);
?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Options')?></td>
        <td><?php echo @$virt_host_dirs[$virt_host_dir][pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")]?></td>
        <td>
          <a href="<?php echo $LINK_URL?>&action=mod&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")?>&oldvalue=<?php echo $virt_host_dirs[$virt_host_dir][pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")]?>"><?php echo $img["mod"]?></a>
<?php if(@$virt_host_dirs[$virt_host_dir][pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")]) { ?>
          <a href="<?php echo $LINK_URL?>&action=del&attrib=<?php echo pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")?>&oldvalue=<?php echo $virt_host_dirs[$virt_host_dir][pql_get_define("PQL_ATTR_WEBSRV_SRV_OPTIONS")]?>"><?php echo $img["del"]?></a>
<?php } ?>
        </td>
      </tr>
    </th>

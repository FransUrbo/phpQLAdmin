  <table cellspacing="0" border="0" width="100%" cellpadding="0">
{foreach name=outer item=button from=$buttons}
  {foreach key=key item=value from=$button}
    <a href="{$url}view={$key}{if isset($link)}&{$link}{/if}"><img alt="/ {$value} \" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label={$value}"></a>
  {/foreach}
{/foreach}
  </table>

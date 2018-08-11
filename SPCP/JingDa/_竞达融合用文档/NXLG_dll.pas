

{ �����Ƭ���� }
function ClearCardData(const Port, Baud: Integer): Integer; stdcall;
{ �������ÿ� }
function MakeSettingCard(const Port, Baud: Integer; const PriceOne, PriceTwo,
  PriceThree, BaseMoney: Double; const PriceOneQuantity, PriceTwoQuantity,
  WarningMoney: Integer): Integer; stdcall;
{ �������㿨 }
function MakeCleanAwayCard(const Port, Baud: Integer): Integer; stdcall;
{ ������鿨 }
function MakeCheckCard(const Port, Baud: Integer): Integer; stdcall;
{ ����Уʱ�� }
function MakeCollateTimeCard(const Port, Baud: Integer;
  SysTime: TDateTime): Integer; stdcall;
{ �������޸���Ϣ�û����������� }
function MakeSpecialUserCard(const Port, Baud: Integer;
  const BuyTimes, WarningMoney: Integer; const MeterSN: Integer;
  const BuyMoney, PriceOne, PriceTwo, PriceThree, PriceOneQuantity,
  PriceTwoQuantity, BaseMoney, BuyTotal: Double; IsReplace: Integer): Integer;
  stdcall;
{ �����û��� ����ֵ�� }
function MakeUserCard(const Port, Baud: Integer; const BuyTimes: Integer;
  const MeterSN: Integer; const BuyMoney: Double): Integer; stdcall;
{ �������� }
function MakeChangeMeterCard(const Port, Baud: Integer): Integer; stdcall;
{ �������� }
function MakeReadingCard(const Port, Baud: Integer): Integer; stdcall;
{ ��ȡ������ }
function GetCardType(const Port, Baud: Integer;
  var CardType: Integer): Integer; stdcall;
{ ��ȡ�û������� }
function ReadUserCardData(const Port, Baud: Integer; var BuyTimes,
  PriceOneQuantity, PriceTwoQuantity, MeterState, WarningMoney: Integer;
  var MeterSN: Integer; var BuyMoney, UseTotal, Remain, Overdraw,
  MonthSum12, MonthSum11, MonthSum10, MonthSum9, MonthSum8, MonthSum7,
  MonthSum6, MonthSum5, MonthSum4, MonthSum3, MonthSum2, MonthSum1, PriceOne,
  PriceTwo, PriceThree, BaseMoney, BuyTotal: Double;
  var CutInCardDate: TDateTime): Integer; stdcall;
{ ��ȡ���ÿ����� }
function ReadSettingCardData(const Port, Baud: Integer; var PriceOne, PriceTwo,
  PriceThree, BaseMoney: Double; var PriceOneQuantity, PriceTwoQuantity,
  WarningMoney: Integer): Integer; stdcall;
{ ��ȡ��鿨���� }
function ReadCheckCardData(const Port, Baud: Integer; var MeterSN: Integer;
  var UseTotal, BuyMoney, Remain, PriceOne, PriceTwo, PriceThree,
  BuyTotal: Double; var PriceOneQuantity, PriceTwoQuantity, UseQuantityTotal,
  MonthSum1, MeterState: Integer): Integer; stdcall;
{ ��ȡ�������� }
function ReadReadingCardData(const Port, Baud: Integer; var MeterSN,
  MonthSum1: Integer): Integer; stdcall;
{ ��ȡУʱ������ }
function ReadCollateTimeCardData(const Port, Baud: Integer;
  var SysTime: TDateTime): Integer; stdcall;
{ ��ȡ�������� }
function ReadChangeMeterCardData(const Port, Baud: Integer; var BuyTimes,
  PriceOneQuantity, PriceTwoQuantity, MeterState, WarningMoney: Integer;
  var MeterSN: Integer; var BuyMoney, UseTotal, Remain, Overdraw,
  MonthSum12, MonthSum11, MonthSum10, MonthSum9, MonthSum8, MonthSum7,
  MonthSum6, MonthSum5, MonthSum4, MonthSum3, MonthSum2, MonthSum1, PriceOne,
  PriceTwo, PriceThree, BaseMoney, BuyTotal: Double;
  var CutInCardDate: TDateTime): Integer; stdcall;




//   Ergasia Gia to Ma8ima Diktya 5ou Examinou 2007
//    Foititis :
//		Papikas Iwannis
//    A.M.     :
//		48/06


simple PC
    gates:
        in: in[];
        out: out[];
endsimple

module PacketFilter1
    submodules:
        PC: PC[4];
            display: "i=device/pc3";
    //;p=176,160
    connections:
        PC[0].out++ --> delay 100ms --> PC[2].in++;
        PC[2].out++ --> delay 100ms --> PC[0].in++;

        PC[1].out++ --> delay 100ms --> PC[2].in++;
        PC[2].out++ --> delay 100ms --> PC[1].in++;

        PC[2].out++ --> delay 100ms --> PC[3].in++;
        PC[3].out++ --> delay 100ms --> PC[2].in++;
endmodule

network PacketFilter : PacketFilter1

endnetwork


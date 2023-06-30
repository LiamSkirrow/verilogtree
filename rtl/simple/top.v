module top();

mod0 mod0_inst0();
mod1 mod1_inst0();
mod2 mod2_inst0();

endmodule

/* 

top 
|___mod0
|   |__mod3
|   |  |__mod4
|   |__mod1
|      |__mod3
|         |__mod4
|___mod1
|   |__mod3
|      |__mod4
|___mod2
    |__mod1
        |__mod3
            |__mod4

altTop
|___mod2
    |__mod1
       |__mod3
          |__mod4

*/
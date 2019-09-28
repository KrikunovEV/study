import tensorflow as tf

mul_op = tf.multiply(2, 3)
sub_op = tf.subtract(mul_op, 3)

with tf.control_dependencies([mul_op, sub_op]):
    div_op = tf.divide(sub_op, mul_op)

output_op = tf.add(div_op, div_op)

session = tf.Session()
output = session.run(output_op)

print(output)
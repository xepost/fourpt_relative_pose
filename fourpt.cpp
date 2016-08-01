transformations_t fourpt(
    const RelativeAdapterBase & adapter,
    const Indices & indices )
{
  

  size_t numberCorrespondences = indices.size();

  MatrixXd A(4,8); 

  Matrix3d rotation = adapter.getR12();

  double pitch1 = rotation(0,0);
  double roll1 = rotation(0,1);
  double yaw1 = rotation(0,2);

  double pitch2 = rotation(1,0);
  double roll2 = rotation(1,1);
  double yaw2 = rotation(1,2);

  Matrix3d rotation1 = RPY2mat(roll1, pitch1, yaw1);
  Matrix3d rotation2 = RPY2mat(roll2, pitch2, yaw2);
 
  for( size_t i = 0; i < numberCorrespondences; i++ )
  {
    bearingVector_t d1 = adapter.getBearingVector1(indices[i]);
    bearingVector_t d2 = adapter.getBearingVector2(indices[i]);
    translation_t v1 = adapter.getCamOffset1(indices[i]);
    translation_t v2 = adapter.getCamOffset2(indices[i]);
    rotation_t R11 = adapter.getCamRotation1(indices[i]);
    rotation_t R21 = adapter.getCamRotation2(indices[i]);

    d1 = R11*d1;
    d2 = R21*d2;

    Eigen::Matrix<double,6,1> l1;
    l1.block<3,1>(0,0) = rotation1*d1;
    l1.block<3,1>(3,0) = rotation1*(v1.cross(d1));
    Eigen::Matrix<double,6,1> l2_pr;
    l2_pr.block<3,1>(0,0) = rotation2*d2;
    l2_pr.block<3,1>(3,0) = rotation2*(v2.cross(d2));

    A(i,0) = l1(0)*l2_pr(3) + l1(1)*l2_pr(4) + l1(2)*l2_pr(5) + 
              l1(3)*l2_pr(0) + l1(4)*l2_pr(1) + l1(5)*l2_pr(2);

    A(i,1) = l1(2)*l2_pr(1) - l1(1)*l2_pr(2);
    A(i,2) = l1(0)*l2_pr(2) - l1(2)*l2_pr(0);
    A(i,3) = l1(1)*l2_pr(0) - l1(0)*l2_pr(1);
    A(i,4) = l1(1)*l2_pr(3) - l1(3)*l2_pr(1) + l1(4)*l2_pr(0) - l1(0)*l2_pr(4);
    A(i,5) = l1(2)*l2_pr(0);
    A(i,6) = l1(2)*l2_pr(1);
    A(i,7) = - l1(0)*l2_pr(0) - l1(1)*l2_pr(1);
  }

  double t0 = -A(0,0)*A(1,1)*A(2,2)*A(3,3)+A(0,0)*A(1,1)*A(2,3)*A(3,2)+A(0,0)*A(1,2)*A(2,1)*A(3,3)-A(0,0)*A(1,2)*A(2,3)*A(3,1)-A(0,0)*A(1,3)*A(2,1)*A(3,2)+A(0,0)*A(1,3)*A(2,2)*A(3,1)+A(0,1)*A(1,0)*A(2,2)*A(3,3)-A(0,1)*A(1,0)*A(2,3)*A(3,2)-A(0,1)*A(1,2)*A(2,0)*A(3,3)+A(0,1)*A(1,2)*A(2,3)*A(3,0)+A(0,1)*A(1,3)*A(2,0)*A(3,2)-A(0,1)*A(1,3)*A(2,2)*A(3,0)-A(0,2)*A(1,0)*A(2,1)*A(3,3)+A(0,2)*A(1,0)*A(2,3)*A(3,1)+A(0,2)*A(1,1)*A(2,0)*A(3,3)-A(0,2)*A(1,1)*A(2,3)*A(3,0)-A(0,2)*A(1,3)*A(2,0)*A(3,1)+A(0,2)*A(1,3)*A(2,1)*A(3,0)+A(0,3)*A(1,0)*A(2,1)*A(3,2)-A(0,3)*A(1,0)*A(2,2)*A(3,1)-A(0,3)*A(1,1)*A(2,0)*A(3,2)+A(0,3)*A(1,1)*A(2,2)*A(3,0)+A(0,3)*A(1,2)*A(2,0)*A(3,1)-A(0,3)*A(1,2)*A(2,1)*A(3,0);
  double t1 = -A(0,0)*A(1,1)*A(2,2)*A(3,7)+A(0,0)*A(1,1)*A(2,3)*A(3,6)-A(0,0)*A(1,1)*A(2,6)*A(3,3)+A(0,0)*A(1,1)*A(2,7)*A(3,2)+A(0,0)*A(1,2)*A(2,1)*A(3,7)-A(0,0)*A(1,2)*A(2,3)*A(3,5)+A(0,0)*A(1,2)*A(2,5)*A(3,3)-A(0,0)*A(1,2)*A(2,7)*A(3,1)-A(0,0)*A(1,3)*A(2,1)*A(3,6)+A(0,0)*A(1,3)*A(2,2)*A(3,5)-A(0,0)*A(1,3)*A(2,5)*A(3,2)+A(0,0)*A(1,3)*A(2,6)*A(3,1)-A(0,0)*A(1,5)*A(2,2)*A(3,3)+A(0,0)*A(1,5)*A(2,3)*A(3,2)+A(0,0)*A(1,6)*A(2,1)*A(3,3)-A(0,0)*A(1,6)*A(2,3)*A(3,1)-A(0,0)*A(1,7)*A(2,1)*A(3,2)+A(0,0)*A(1,7)*A(2,2)*A(3,1)+A(0,1)*A(1,0)*A(2,2)*A(3,7)-A(0,1)*A(1,0)*A(2,3)*A(3,6)+A(0,1)*A(1,0)*A(2,6)*A(3,3)-A(0,1)*A(1,0)*A(2,7)*A(3,2)-A(0,1)*A(1,2)*A(2,0)*A(3,7)+A(0,1)*A(1,2)*A(2,3)*A(3,4)-A(0,1)*A(1,2)*A(2,4)*A(3,3)+A(0,1)*A(1,2)*A(2,7)*A(3,0)+A(0,1)*A(1,3)*A(2,0)*A(3,6)-A(0,1)*A(1,3)*A(2,2)*A(3,4)+A(0,1)*A(1,3)*A(2,4)*A(3,2)-A(0,1)*A(1,3)*A(2,6)*A(3,0)+A(0,1)*A(1,4)*A(2,2)*A(3,3)-A(0,1)*A(1,4)*A(2,3)*A(3,2)-A(0,1)*A(1,6)*A(2,0)*A(3,3)+A(0,1)*A(1,6)*A(2,3)*A(3,0)+A(0,1)*A(1,7)*A(2,0)*A(3,2)-A(0,1)*A(1,7)*A(2,2)*A(3,0)-A(0,2)*A(1,0)*A(2,1)*A(3,7)+A(0,2)*A(1,0)*A(2,3)*A(3,5)-A(0,2)*A(1,0)*A(2,5)*A(3,3)+A(0,2)*A(1,0)*A(2,7)*A(3,1)+A(0,2)*A(1,1)*A(2,0)*A(3,7)-A(0,2)*A(1,1)*A(2,3)*A(3,4)+A(0,2)*A(1,1)*A(2,4)*A(3,3)-A(0,2)*A(1,1)*A(2,7)*A(3,0)-A(0,2)*A(1,3)*A(2,0)*A(3,5)+A(0,2)*A(1,3)*A(2,1)*A(3,4)-A(0,2)*A(1,3)*A(2,4)*A(3,1)+A(0,2)*A(1,3)*A(2,5)*A(3,0)-A(0,2)*A(1,4)*A(2,1)*A(3,3)+A(0,2)*A(1,4)*A(2,3)*A(3,1)+A(0,2)*A(1,5)*A(2,0)*A(3,3)-A(0,2)*A(1,5)*A(2,3)*A(3,0)-A(0,2)*A(1,7)*A(2,0)*A(3,1)+A(0,2)*A(1,7)*A(2,1)*A(3,0)+A(0,3)*A(1,0)*A(2,1)*A(3,6)-A(0,3)*A(1,0)*A(2,2)*A(3,5)+A(0,3)*A(1,0)*A(2,5)*A(3,2)-A(0,3)*A(1,0)*A(2,6)*A(3,1)-A(0,3)*A(1,1)*A(2,0)*A(3,6)+A(0,3)*A(1,1)*A(2,2)*A(3,4)-A(0,3)*A(1,1)*A(2,4)*A(3,2)+A(0,3)*A(1,1)*A(2,6)*A(3,0)+A(0,3)*A(1,2)*A(2,0)*A(3,5)-A(0,3)*A(1,2)*A(2,1)*A(3,4)+A(0,3)*A(1,2)*A(2,4)*A(3,1)-A(0,3)*A(1,2)*A(2,5)*A(3,0)+A(0,3)*A(1,4)*A(2,1)*A(3,2)-A(0,3)*A(1,4)*A(2,2)*A(3,1)-A(0,3)*A(1,5)*A(2,0)*A(3,2)+A(0,3)*A(1,5)*A(2,2)*A(3,0)+A(0,3)*A(1,6)*A(2,0)*A(3,1)-A(0,3)*A(1,6)*A(2,1)*A(3,0)-A(0,4)*A(1,1)*A(2,2)*A(3,3)+A(0,4)*A(1,1)*A(2,3)*A(3,2)+A(0,4)*A(1,2)*A(2,1)*A(3,3)-A(0,4)*A(1,2)*A(2,3)*A(3,1)-A(0,4)*A(1,3)*A(2,1)*A(3,2)+A(0,4)*A(1,3)*A(2,2)*A(3,1)+A(0,5)*A(1,0)*A(2,2)*A(3,3)-A(0,5)*A(1,0)*A(2,3)*A(3,2)-A(0,5)*A(1,2)*A(2,0)*A(3,3)+A(0,5)*A(1,2)*A(2,3)*A(3,0)+A(0,5)*A(1,3)*A(2,0)*A(3,2)-A(0,5)*A(1,3)*A(2,2)*A(3,0)-A(0,6)*A(1,0)*A(2,1)*A(3,3)+A(0,6)*A(1,0)*A(2,3)*A(3,1)+A(0,6)*A(1,1)*A(2,0)*A(3,3)-A(0,6)*A(1,1)*A(2,3)*A(3,0)-A(0,6)*A(1,3)*A(2,0)*A(3,1)+A(0,6)*A(1,3)*A(2,1)*A(3,0)+A(0,7)*A(1,0)*A(2,1)*A(3,2)-A(0,7)*A(1,0)*A(2,2)*A(3,1)-A(0,7)*A(1,1)*A(2,0)*A(3,2)+A(0,7)*A(1,1)*A(2,2)*A(3,0)+A(0,7)*A(1,2)*A(2,0)*A(3,1)-A(0,7)*A(1,2)*A(2,1)*A(3,0);
  double t2 = -A(0,0)*A(1,1)*A(2,6)*A(3,7)+A(0,0)*A(1,1)*A(2,7)*A(3,6)+A(0,0)*A(1,2)*A(2,5)*A(3,7)-A(0,0)*A(1,2)*A(2,7)*A(3,5)-A(0,0)*A(1,3)*A(2,5)*A(3,6)+A(0,0)*A(1,3)*A(2,6)*A(3,5)-A(0,0)*A(1,5)*A(2,2)*A(3,7)+A(0,0)*A(1,5)*A(2,3)*A(3,6)-A(0,0)*A(1,5)*A(2,6)*A(3,3)+A(0,0)*A(1,5)*A(2,7)*A(3,2)+A(0,0)*A(1,6)*A(2,1)*A(3,7)-A(0,0)*A(1,6)*A(2,3)*A(3,5)+A(0,0)*A(1,6)*A(2,5)*A(3,3)-A(0,0)*A(1,6)*A(2,7)*A(3,1)-A(0,0)*A(1,7)*A(2,1)*A(3,6)+A(0,0)*A(1,7)*A(2,2)*A(3,5)-A(0,0)*A(1,7)*A(2,5)*A(3,2)+A(0,0)*A(1,7)*A(2,6)*A(3,1)+A(0,1)*A(1,0)*A(2,6)*A(3,7)-A(0,1)*A(1,0)*A(2,7)*A(3,6)-A(0,1)*A(1,2)*A(2,4)*A(3,7)+A(0,1)*A(1,2)*A(2,7)*A(3,4)+A(0,1)*A(1,3)*A(2,4)*A(3,6)-A(0,1)*A(1,3)*A(2,6)*A(3,4)+A(0,1)*A(1,4)*A(2,2)*A(3,7)-A(0,1)*A(1,4)*A(2,3)*A(3,6)+A(0,1)*A(1,4)*A(2,6)*A(3,3)-A(0,1)*A(1,4)*A(2,7)*A(3,2)-A(0,1)*A(1,6)*A(2,0)*A(3,7)+A(0,1)*A(1,6)*A(2,3)*A(3,4)-A(0,1)*A(1,6)*A(2,4)*A(3,3)+A(0,1)*A(1,6)*A(2,7)*A(3,0)+A(0,1)*A(1,7)*A(2,0)*A(3,6)-A(0,1)*A(1,7)*A(2,2)*A(3,4)+A(0,1)*A(1,7)*A(2,4)*A(3,2)-A(0,1)*A(1,7)*A(2,6)*A(3,0)-A(0,2)*A(1,0)*A(2,5)*A(3,7)+A(0,2)*A(1,0)*A(2,7)*A(3,5)+A(0,2)*A(1,1)*A(2,4)*A(3,7)-A(0,2)*A(1,1)*A(2,7)*A(3,4)-A(0,2)*A(1,3)*A(2,4)*A(3,5)+A(0,2)*A(1,3)*A(2,5)*A(3,4)-A(0,2)*A(1,4)*A(2,1)*A(3,7)+A(0,2)*A(1,4)*A(2,3)*A(3,5)-A(0,2)*A(1,4)*A(2,5)*A(3,3)+A(0,2)*A(1,4)*A(2,7)*A(3,1)+A(0,2)*A(1,5)*A(2,0)*A(3,7)-A(0,2)*A(1,5)*A(2,3)*A(3,4)+A(0,2)*A(1,5)*A(2,4)*A(3,3)-A(0,2)*A(1,5)*A(2,7)*A(3,0)-A(0,2)*A(1,7)*A(2,0)*A(3,5)+A(0,2)*A(1,7)*A(2,1)*A(3,4)-A(0,2)*A(1,7)*A(2,4)*A(3,1)+A(0,2)*A(1,7)*A(2,5)*A(3,0)+A(0,3)*A(1,0)*A(2,5)*A(3,6)-A(0,3)*A(1,0)*A(2,6)*A(3,5)-A(0,3)*A(1,1)*A(2,4)*A(3,6)+A(0,3)*A(1,1)*A(2,6)*A(3,4)+A(0,3)*A(1,2)*A(2,4)*A(3,5)-A(0,3)*A(1,2)*A(2,5)*A(3,4)+A(0,3)*A(1,4)*A(2,1)*A(3,6)-A(0,3)*A(1,4)*A(2,2)*A(3,5)+A(0,3)*A(1,4)*A(2,5)*A(3,2)-A(0,3)*A(1,4)*A(2,6)*A(3,1)-A(0,3)*A(1,5)*A(2,0)*A(3,6)+A(0,3)*A(1,5)*A(2,2)*A(3,4)-A(0,3)*A(1,5)*A(2,4)*A(3,2)+A(0,3)*A(1,5)*A(2,6)*A(3,0)+A(0,3)*A(1,6)*A(2,0)*A(3,5)-A(0,3)*A(1,6)*A(2,1)*A(3,4)+A(0,3)*A(1,6)*A(2,4)*A(3,1)-A(0,3)*A(1,6)*A(2,5)*A(3,0)-A(0,4)*A(1,1)*A(2,2)*A(3,7)+A(0,4)*A(1,1)*A(2,3)*A(3,6)-A(0,4)*A(1,1)*A(2,6)*A(3,3)+A(0,4)*A(1,1)*A(2,7)*A(3,2)+A(0,4)*A(1,2)*A(2,1)*A(3,7)-A(0,4)*A(1,2)*A(2,3)*A(3,5)+A(0,4)*A(1,2)*A(2,5)*A(3,3)-A(0,4)*A(1,2)*A(2,7)*A(3,1)-A(0,4)*A(1,3)*A(2,1)*A(3,6)+A(0,4)*A(1,3)*A(2,2)*A(3,5)-A(0,4)*A(1,3)*A(2,5)*A(3,2)+A(0,4)*A(1,3)*A(2,6)*A(3,1)-A(0,4)*A(1,5)*A(2,2)*A(3,3)+A(0,4)*A(1,5)*A(2,3)*A(3,2)+A(0,4)*A(1,6)*A(2,1)*A(3,3)-A(0,4)*A(1,6)*A(2,3)*A(3,1)-A(0,4)*A(1,7)*A(2,1)*A(3,2)+A(0,4)*A(1,7)*A(2,2)*A(3,1)+A(0,5)*A(1,0)*A(2,2)*A(3,7)-A(0,5)*A(1,0)*A(2,3)*A(3,6)+A(0,5)*A(1,0)*A(2,6)*A(3,3)-A(0,5)*A(1,0)*A(2,7)*A(3,2)-A(0,5)*A(1,2)*A(2,0)*A(3,7)+A(0,5)*A(1,2)*A(2,3)*A(3,4)-A(0,5)*A(1,2)*A(2,4)*A(3,3)+A(0,5)*A(1,2)*A(2,7)*A(3,0)+A(0,5)*A(1,3)*A(2,0)*A(3,6)-A(0,5)*A(1,3)*A(2,2)*A(3,4)+A(0,5)*A(1,3)*A(2,4)*A(3,2)-A(0,5)*A(1,3)*A(2,6)*A(3,0)+A(0,5)*A(1,4)*A(2,2)*A(3,3)-A(0,5)*A(1,4)*A(2,3)*A(3,2)-A(0,5)*A(1,6)*A(2,0)*A(3,3)+A(0,5)*A(1,6)*A(2,3)*A(3,0)+A(0,5)*A(1,7)*A(2,0)*A(3,2)-A(0,5)*A(1,7)*A(2,2)*A(3,0)-A(0,6)*A(1,0)*A(2,1)*A(3,7)+A(0,6)*A(1,0)*A(2,3)*A(3,5)-A(0,6)*A(1,0)*A(2,5)*A(3,3)+A(0,6)*A(1,0)*A(2,7)*A(3,1)+A(0,6)*A(1,1)*A(2,0)*A(3,7)-A(0,6)*A(1,1)*A(2,3)*A(3,4)+A(0,6)*A(1,1)*A(2,4)*A(3,3)-A(0,6)*A(1,1)*A(2,7)*A(3,0)-A(0,6)*A(1,3)*A(2,0)*A(3,5)+A(0,6)*A(1,3)*A(2,1)*A(3,4)-A(0,6)*A(1,3)*A(2,4)*A(3,1)+A(0,6)*A(1,3)*A(2,5)*A(3,0)-A(0,6)*A(1,4)*A(2,1)*A(3,3)+A(0,6)*A(1,4)*A(2,3)*A(3,1)+A(0,6)*A(1,5)*A(2,0)*A(3,3)-A(0,6)*A(1,5)*A(2,3)*A(3,0)-A(0,6)*A(1,7)*A(2,0)*A(3,1)+A(0,6)*A(1,7)*A(2,1)*A(3,0)+A(0,7)*A(1,0)*A(2,1)*A(3,6)-A(0,7)*A(1,0)*A(2,2)*A(3,5)+A(0,7)*A(1,0)*A(2,5)*A(3,2)-A(0,7)*A(1,0)*A(2,6)*A(3,1)-A(0,7)*A(1,1)*A(2,0)*A(3,6)+A(0,7)*A(1,1)*A(2,2)*A(3,4)-A(0,7)*A(1,1)*A(2,4)*A(3,2)+A(0,7)*A(1,1)*A(2,6)*A(3,0)+A(0,7)*A(1,2)*A(2,0)*A(3,5)-A(0,7)*A(1,2)*A(2,1)*A(3,4)+A(0,7)*A(1,2)*A(2,4)*A(3,1)-A(0,7)*A(1,2)*A(2,5)*A(3,0)+A(0,7)*A(1,4)*A(2,1)*A(3,2)-A(0,7)*A(1,4)*A(2,2)*A(3,1)-A(0,7)*A(1,5)*A(2,0)*A(3,2)+A(0,7)*A(1,5)*A(2,2)*A(3,0)+A(0,7)*A(1,6)*A(2,0)*A(3,1)-A(0,7)*A(1,6)*A(2,1)*A(3,0);
  double t3 = -A(0,0)*A(1,5)*A(2,6)*A(3,7)+A(0,0)*A(1,5)*A(2,7)*A(3,6)+A(0,0)*A(1,6)*A(2,5)*A(3,7)-A(0,0)*A(1,6)*A(2,7)*A(3,5)-A(0,0)*A(1,7)*A(2,5)*A(3,6)+A(0,0)*A(1,7)*A(2,6)*A(3,5)+A(0,1)*A(1,4)*A(2,6)*A(3,7)-A(0,1)*A(1,4)*A(2,7)*A(3,6)-A(0,1)*A(1,6)*A(2,4)*A(3,7)+A(0,1)*A(1,6)*A(2,7)*A(3,4)+A(0,1)*A(1,7)*A(2,4)*A(3,6)-A(0,1)*A(1,7)*A(2,6)*A(3,4)-A(0,2)*A(1,4)*A(2,5)*A(3,7)+A(0,2)*A(1,4)*A(2,7)*A(3,5)+A(0,2)*A(1,5)*A(2,4)*A(3,7)-A(0,2)*A(1,5)*A(2,7)*A(3,4)-A(0,2)*A(1,7)*A(2,4)*A(3,5)+A(0,2)*A(1,7)*A(2,5)*A(3,4)+A(0,3)*A(1,4)*A(2,5)*A(3,6)-A(0,3)*A(1,4)*A(2,6)*A(3,5)-A(0,3)*A(1,5)*A(2,4)*A(3,6)+A(0,3)*A(1,5)*A(2,6)*A(3,4)+A(0,3)*A(1,6)*A(2,4)*A(3,5)-A(0,3)*A(1,6)*A(2,5)*A(3,4)-A(0,4)*A(1,1)*A(2,6)*A(3,7)+A(0,4)*A(1,1)*A(2,7)*A(3,6)+A(0,4)*A(1,2)*A(2,5)*A(3,7)-A(0,4)*A(1,2)*A(2,7)*A(3,5)-A(0,4)*A(1,3)*A(2,5)*A(3,6)+A(0,4)*A(1,3)*A(2,6)*A(3,5)-A(0,4)*A(1,5)*A(2,2)*A(3,7)+A(0,4)*A(1,5)*A(2,3)*A(3,6)-A(0,4)*A(1,5)*A(2,6)*A(3,3)+A(0,4)*A(1,5)*A(2,7)*A(3,2)+A(0,4)*A(1,6)*A(2,1)*A(3,7)-A(0,4)*A(1,6)*A(2,3)*A(3,5)+A(0,4)*A(1,6)*A(2,5)*A(3,3)-A(0,4)*A(1,6)*A(2,7)*A(3,1)-A(0,4)*A(1,7)*A(2,1)*A(3,6)+A(0,4)*A(1,7)*A(2,2)*A(3,5)-A(0,4)*A(1,7)*A(2,5)*A(3,2)+A(0,4)*A(1,7)*A(2,6)*A(3,1)+A(0,5)*A(1,0)*A(2,6)*A(3,7)-A(0,5)*A(1,0)*A(2,7)*A(3,6)-A(0,5)*A(1,2)*A(2,4)*A(3,7)+A(0,5)*A(1,2)*A(2,7)*A(3,4)+A(0,5)*A(1,3)*A(2,4)*A(3,6)-A(0,5)*A(1,3)*A(2,6)*A(3,4)+A(0,5)*A(1,4)*A(2,2)*A(3,7)-A(0,5)*A(1,4)*A(2,3)*A(3,6)+A(0,5)*A(1,4)*A(2,6)*A(3,3)-A(0,5)*A(1,4)*A(2,7)*A(3,2)-A(0,5)*A(1,6)*A(2,0)*A(3,7)+A(0,5)*A(1,6)*A(2,3)*A(3,4)-A(0,5)*A(1,6)*A(2,4)*A(3,3)+A(0,5)*A(1,6)*A(2,7)*A(3,0)+A(0,5)*A(1,7)*A(2,0)*A(3,6)-A(0,5)*A(1,7)*A(2,2)*A(3,4)+A(0,5)*A(1,7)*A(2,4)*A(3,2)-A(0,5)*A(1,7)*A(2,6)*A(3,0)-A(0,6)*A(1,0)*A(2,5)*A(3,7)+A(0,6)*A(1,0)*A(2,7)*A(3,5)+A(0,6)*A(1,1)*A(2,4)*A(3,7)-A(0,6)*A(1,1)*A(2,7)*A(3,4)-A(0,6)*A(1,3)*A(2,4)*A(3,5)+A(0,6)*A(1,3)*A(2,5)*A(3,4)-A(0,6)*A(1,4)*A(2,1)*A(3,7)+A(0,6)*A(1,4)*A(2,3)*A(3,5)-A(0,6)*A(1,4)*A(2,5)*A(3,3)+A(0,6)*A(1,4)*A(2,7)*A(3,1)+A(0,6)*A(1,5)*A(2,0)*A(3,7)-A(0,6)*A(1,5)*A(2,3)*A(3,4)+A(0,6)*A(1,5)*A(2,4)*A(3,3)-A(0,6)*A(1,5)*A(2,7)*A(3,0)-A(0,6)*A(1,7)*A(2,0)*A(3,5)+A(0,6)*A(1,7)*A(2,1)*A(3,4)-A(0,6)*A(1,7)*A(2,4)*A(3,1)+A(0,6)*A(1,7)*A(2,5)*A(3,0)+A(0,7)*A(1,0)*A(2,5)*A(3,6)-A(0,7)*A(1,0)*A(2,6)*A(3,5)-A(0,7)*A(1,1)*A(2,4)*A(3,6)+A(0,7)*A(1,1)*A(2,6)*A(3,4)+A(0,7)*A(1,2)*A(2,4)*A(3,5)-A(0,7)*A(1,2)*A(2,5)*A(3,4)+A(0,7)*A(1,4)*A(2,1)*A(3,6)-A(0,7)*A(1,4)*A(2,2)*A(3,5)+A(0,7)*A(1,4)*A(2,5)*A(3,2)-A(0,7)*A(1,4)*A(2,6)*A(3,1)-A(0,7)*A(1,5)*A(2,0)*A(3,6)+A(0,7)*A(1,5)*A(2,2)*A(3,4)-A(0,7)*A(1,5)*A(2,4)*A(3,2)+A(0,7)*A(1,5)*A(2,6)*A(3,0)+A(0,7)*A(1,6)*A(2,0)*A(3,5)-A(0,7)*A(1,6)*A(2,1)*A(3,4)+A(0,7)*A(1,6)*A(2,4)*A(3,1)-A(0,7)*A(1,6)*A(2,5)*A(3,0);
  double t4 = -A(0,4)*A(1,5)*A(2,6)*A(3,7)+A(0,4)*A(1,5)*A(2,7)*A(3,6)+A(0,4)*A(1,6)*A(2,5)*A(3,7)-A(0,4)*A(1,6)*A(2,7)*A(3,5)-A(0,4)*A(1,7)*A(2,5)*A(3,6)+A(0,4)*A(1,7)*A(2,6)*A(3,5)+A(0,5)*A(1,4)*A(2,6)*A(3,7)-A(0,5)*A(1,4)*A(2,7)*A(3,6)-A(0,5)*A(1,6)*A(2,4)*A(3,7)+A(0,5)*A(1,6)*A(2,7)*A(3,4)+A(0,5)*A(1,7)*A(2,4)*A(3,6)-A(0,5)*A(1,7)*A(2,6)*A(3,4)-A(0,6)*A(1,4)*A(2,5)*A(3,7)+A(0,6)*A(1,4)*A(2,7)*A(3,5)+A(0,6)*A(1,5)*A(2,4)*A(3,7)-A(0,6)*A(1,5)*A(2,7)*A(3,4)-A(0,6)*A(1,7)*A(2,4)*A(3,5)+A(0,6)*A(1,7)*A(2,5)*A(3,4)+A(0,7)*A(1,4)*A(2,5)*A(3,6)-A(0,7)*A(1,4)*A(2,6)*A(3,5)-A(0,7)*A(1,5)*A(2,4)*A(3,6)+A(0,7)*A(1,5)*A(2,6)*A(3,4)+A(0,7)*A(1,6)*A(2,4)*A(3,5)-A(0,7)*A(1,6)*A(2,5)*A(3,4);


  double a3 = t3/t4;
  double a2 = t2/t4;
  double a1 = t1/t4;
  double a0 = t0/t4;

  complex<double> T1 = -a3/4.0;
  complex<double> T2 = a2*a2 - 3.0*a3*a1 + 12.0*a0;
  complex<double> T3 = (2.0*a2*a2*a2 - 9.0*a3*a2*a1 + 27.0*a1*a1 + 27.0*a3*a3*a0 - 72.0*a2*a0)/2.0;
  complex<double> T4 = (-a3*a3*a3 + 4.0*a3*a2 - 8.0*a1)/32.0;
  complex<double> T5 = (3.0*a3*a3 - 8.0*a2)/48.0;
  complex<double> R1 = sqrt(T3*T3 - T2*T2*T2);
  complex<double> T3R1 = T3 + R1;
  complex<double> R2;
  if (T3R1.imag() == 0.0 && T3R1.real() < 0.0)
  {
    R2 = - pow(abs(T3R1), 1.0/3.0);
  }
  else
  {
    R2 = pow(T3R1, 1.0/3.0);
  }
  complex<double> R3 = (1.0/12.0)*(T2/R2 + R2);
  complex<double> R4 = sqrt(T5 + R3);
  complex<double> R5 = 2.0 *T5 - R3;
  complex<double> R6 = T4 / R4;
  if ((T4 == 0.0) && (T5 == 0.0) && (abs(R3) < 1e-16))
    R6 = 1.0;

  complex<double> r[4];
  r[0] = T1 - R4 - sqrt(R5 - R6);
  r[1] = T1 - R4 + sqrt(R5 - R6);
  r[2] = T1 + R4 - sqrt(R5 + R6);
  r[3] = T1 + R4 + sqrt(R5 + R6);

  // extract the real root
  std::vector <double> real_roots;
  for (int i = 0; i < 4; ++i)
  {
    if ((abs(r[i].imag()) < 1e-6) && (abs(r[i].real()) < 0.2618)) // max 15 degree
    {
      real_roots.push_back(r[i].real());
    }
  }

  transformations_t RTS;
  RTS.resize(real_roots.size());
  for (int i = 0; i < real_roots.size(); ++i)
  {

    Matrix3d skew;
    skew << 0.0, -real_roots[i], 0.0,
             real_roots[i], 0.0, 0.0,
             0.0 ,0.0, 0.0;

    Matrix3d yaw_m;
    
    if (real_roots[i] == 0.0 )
    {
      yaw_m = Matrix3d::Identity();
    }
    else
    {
      yaw_m = Matrix3d::Identity() + (sin(abs(real_roots[i]))/abs(real_roots[i])) * skew +
              ((1- cos(abs(real_roots[i])))/ (real_roots[i] * real_roots[i])) * skew * skew;
    } 

    Matrix<double,4,3> M_c;
    Vector4d b_c;

    for (int j = 0; j < 4; ++j)
    {
      M_c(j,0) = A(j,1) + real_roots[i] * A(j,5);
      M_c(j,1) = A(j,2) + real_roots[i] * A(j,6);
      M_c(j,2) = A(j,3) + real_roots[i] * A(j,7);
      b_c(j) = -(A(j,0) + real_roots[i] * A(j,4));
    }

    Vector3d t = M_c.householderQr().solve(b_c);
    Matrix3d R_est = rotation1.transpose()*yaw_m * rotation2;
    transformation_t RT;
    RT.block<3,3>(0,0) = R_est;
    RT.block<3,1>(0,3) = rotation1.transpose()*t;
    RTS.at(i) = RT;
  }

 return RTS;

}

template<typename T>
Eigen::Matrix<T, 3, 3> RPY2mat(T roll, T pitch, T yaw)
{
    Eigen::Matrix<T, 3, 3> m;

    T cr = cos(roll);
    T sr = sin(roll);
    T cp = cos(pitch);
    T sp = sin(pitch);
    T cy = cos(yaw);
    T sy = sin(yaw);

    m(0,0) = cy * cp;
    m(0,1) = cy * sp * sr - sy * cr;
    m(0,2) = cy * sp * cr + sy * sr;
    m(1,0) = sy * cp;
    m(1,1) = sy * sp * sr + cy * cr;
    m(1,2) = sy * sp * cr - cy * sr;
    m(2,0) = - sp;
    m(2,1) = cp * sr;
    m(2,2) = cp * cr;
    return m; 
}
